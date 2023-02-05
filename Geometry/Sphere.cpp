#include <math.h>
#include "Sphere.h"
#include "Math/Vector3D.h"
#include "Parser/Hash.h"
#include "Materials/Material.h"
#include "Math/Maths.h"
#include "Math/Matrix.h"
#include "Textures/Texture.h"
#include "Samplers/Sampler.h"

Sphere::Sphere() :
   GeometryObject(),
   thetaRange(false),
   phiRange(false),
   cosThetaMin(0),
   cosThetaMax(0),
   phiMin(0),
   phiMax(0)
{
   bbox.expand(Point3D(-1, -1, -1));
   bbox.expand(Point3D(1, 1, 1));

   nSamples = 10;
   idx = 0;
   samples = new float[nSamples*2];
   Sampler::LatinHyperCube(samples, nSamples, 2);
}

Sphere::~Sphere() {
   delete[] samples;
}

bool Sphere::hit(const Ray& ray, ShadeRecord& sr) const {
   if(!bbox.hit(ray)) return false;

   Vector3D temp(ray.origin - Point3D());
   double a = ray.direction.dot(ray.direction);
   double b = 2.0 * temp.dot(ray.direction);
   double c = temp.dot(temp) - 1.0;
   double disc = b * b - 4.0 * a * c;

   if(disc < 0.0) {
      return false;
   }

   double e = sqrt(disc);
   double denom = 2.0 * a;
   double t = (-b - e) / denom;

   if(t > epsilon && t < ray.tHit && partCheck(ray, t)) {
      ray.tHit = t;
      sr.normal = (temp + ray.direction * t);

      if((-ray.direction).dot(sr.normal) < 0.0) {
         sr.normal *= -1.0;
      }
      setTangents(sr);

      sr.localHitPoint = sr.hitPoint = ray(t);
      sr.material = material;
      return true;
   }

   t = (-b + e) / denom;
   if(t > epsilon && t < ray.tHit && partCheck(ray, t)) {
      ray.tHit = t;
      sr.normal = (temp + ray.direction * t);

      if((-ray.direction).dot(sr.normal) < 0.0) {
         sr.normal *= -1;
      }
      setTangents(sr);

      sr.localHitPoint = sr.hitPoint = ray(t);
      sr.material = material;
      return true;
   }

   return false;
}

void Sphere::setTangents(ShadeRecord& sr) const {
   if(sr.normal.x == 0 && sr.normal.y == 1 && sr.normal.z == 0) {
      sr.tangent.set(1, 0, 0);
      sr.binormal.set(0, 0, -1);
   }
   else if(sr.normal.x == 0 && sr.normal.y == -1 && sr.normal.z == 0) {
      sr.tangent.set(-1, 0, 0);
      sr.binormal.set(0, 0, 1);
   }
   else {
      sr.tangent.set(sr.normal.z, 0, -sr.normal.x);
      sr.binormal = sr.normal.cross(sr.tangent);
   }

   sr.tangent.normalize();
   sr.binormal.normalize();
}

bool Sphere::shadowHit(const Ray& ray) const {
   if(!bbox.hit(ray)) return false;

   Vector3D temp(ray.origin - Point3D());
   double a = ray.direction.dot(ray.direction);
   double b = 2.0 * temp.dot(ray.direction);
   double c = temp.dot(temp) - 1.0;
   double disc = b * b - 4.0 * a * c;

   if(disc < 0.0) {
      return false;
   }

   double e = sqrt(disc);
   double denom = 2.0 * a;
   double t = (-b - e) / denom;

   if(t > epsilon && partCheck(ray, t)) {
      ShadeRecord sr;
      sr.localHitPoint = ray(t);

      if(material->shadowHit(sr)) {
         ray.tHit = t;
         return true;
      }
   }

   t = (-b + e) / denom;
   if(t > epsilon && partCheck(ray, t)) {
      ShadeRecord sr;
      sr.localHitPoint = ray(t);

      if(material->shadowHit(sr)) {
         ray.tHit = t;
         return true;
      }
   }

   return false;
}

bool Sphere::partCheck(const Ray& ray, double t) const {
   if(!thetaRange && !phiRange) {
      return true;
   }

   Point3D hit = ray(t);

   if(thetaRange) {
      if(hit.y > cosThetaMin || hit.y < cosThetaMax) {
         return false;
      }
   }

   if(phiRange) {
      double phi = atan2(hit.x, hit.z);
      if(phi < 0.0) {
         phi += 2.0 * M_PI;
      }
      if(phi < phiMin || phi > phiMax) {
         return false;
      }
   }
   return true;
}

void Sphere::setHash(Hash* hash) {
   if(hash->contains("thetaRange")) {
      thetaRange = true;
      Array* a = hash->getValue("thetaRange")->getArray();
      double min = a->at(0)->getDouble();
      cosThetaMin = cos(min * DEG_TO_RAD);

      double max = a->at(1)->getDouble();
      cosThetaMax = cos(max * DEG_TO_RAD);
   }

   if(hash->contains("phiRange")) {
      phiRange = true;
      Array* a = hash->getValue("phiRange")->getArray();
      phiMin = a->at(0)->getDouble() * DEG_TO_RAD;
      phiMax = a->at(1)->getDouble() * DEG_TO_RAD;
   }

   if(hash->contains("ignoreShadow")) {
      ignoreShadow = true;
   }

   material = Material::createMaterial(hash->getValue("material")->getHash());
}

Point3D Sphere::sample(const Point3D& hitPoint, float u1, float u2, Vector3D& normal) {
   float x = samples[idx];
   float y = samples[idx+1];
   idx = (idx + 2) % (nSamples * 2);

   // Get a cooredinate system for sphere sampling. z axis is vector from hit point to sphere center
   Vector3D wc = Point3D() - hitPoint;
   wc.normalize();
   Vector3D wcx, wcy;
   coordinateSystem(wc, &wcx, &wcy);

   // Sample sphere inside subtended cone
   double sinTheta = 1.0 / hitPoint.distanceSquared(Point3D());
   double cosTheta = sqrt(max(0.0, 1.0 - sinTheta));

   Ray ray(hitPoint, Sampler::uniformSampleCone(x, y, cosTheta, wcx, wcy, wc));
   if(!shadowHit(ray)) {
      ray.tHit = (Point3D() - hitPoint).dot(ray.direction.normalize());
   }

   normal.set(hitPoint.x, hitPoint.y, hitPoint.z);
   normal.normalize();

   return ray(ray.tHit);
}

double Sphere::pdf(const ShadeRecord& sr) const {
   double sinTheta = 1.0 / sr.hitPoint.distanceSquared(Point3D());
   double cosTheta = sqrt(max(0.0, 1.0 - sinTheta));
   return 1.0 / (2.0 * M_PI * (1.0 - cosTheta));
}

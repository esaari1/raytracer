#include "Disk.h"
#include "Parser/Hash.h"
#include "Samplers/Sampler.h"
#include "Materials/Material.h"
#include <math.h>

Disk::Disk() : GeometryObject() {
}

Disk::~Disk() {
}

void Disk::setHash(Hash* hash) {
   material = Material::createMaterial(hash->getValue("material")->getHash());
}

bool Disk::hit(const Ray& ray, ShadeRecord& sr) const {
   float t = -ray.origin.z / ray.direction.z;

   if(t < epsilon || t > ray.tHit) {
      return false;
   }

   Point3D p = ray(t);
   if(Point3D().distanceSquared(p) < 1.0) {
      ray.tHit = t;
      sr.normal.set(0, 0, 1);
      sr.localHitPoint = sr.hitPoint = p;
      sr.material = material;
      return true;
   }
   return false;
}

bool Disk::shadowHit(const Ray& ray) const {
   float t = -ray.origin.z / ray.direction.z;

   if(t < epsilon) {
      return false;
   }

   Point3D p = ray.origin + ray.direction * t;
   if(Point3D().distanceSquared(p) < 1.0) {
      ray.tHit = t;
      return true;
   }
   return false;
}

Point3D Disk::sample(const Point3D& hitPoint, float u1, float u2, Vector3D& normal) {
   float x, y;
   Sampler::mapToDisk(u1, u2, &x, &y);
   normal.set(0, 0, 1);
   return Point3D(x, y, 0);
}

double Disk::pdf(const ShadeRecord& sr) const {
   return M_1_PI; // 1 / PI
}

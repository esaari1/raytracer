#include "Rectangle.h"
#include "Parser/Hash.h"
#include "Samplers/Sampler.h"
#include "Materials/Material.h"

Rectangle::Rectangle() : GeometryObject() {
   bbox.expand(Point3D(-1, -1, 0));
   bbox.expand(Point3D(1, 1, 0));
}

Rectangle::~Rectangle() {
}

void Rectangle::setHash(Hash* hash) {
   bbox.expand(Point3D(-1, -1, 0));
   bbox.expand(Point3D(1, 1, 0));

   material = Material::createMaterial(hash->getValue("material")->getHash());
}

bool Rectangle::hit(const Ray& ray, ShadeRecord& sr) const {
   double t = -ray.origin.z / ray.direction.z;
   if(t <= epsilon || t > ray.tHit) {
      return false;
   }

   Point3D p = ray(t);

   float ddota = 2.f * (p.x + 1.f);
   if(ddota < 0.0 || ddota > 4.f) {
      return false;
   }

   float ddotb = 2.f * (p.y + 1.f);
   if(ddotb < 0.0 || ddotb > 4.f) {
      return false;
   }

   ray.tHit = t;
   sr.normal.set(0, 0, 1);
   sr.localHitPoint = sr.hitPoint = p;
   sr.material = material;
   sr.tu = ddota / 4.f;
   sr.tv = ddotb / 4.f;

   return true;
}

bool Rectangle::shadowHit(const Ray& ray) const {
   if(ignoreShadow) {
      return false;
   }

   double t = -ray.origin.z / ray.direction.z;
   if(t <= epsilon) {
      return false;
   }

   Point3D p = ray(t);

   float ddota = 2.f * (p.x + 1.f);
   if(ddota < 0.0 || ddota > 4.f) {
      return false;
   }

   float ddotb = 2.f * (p.y + 1.f);
   if(ddotb < 0.0 || ddotb > 4.f) {
      return false;
   }

   ray.tHit = t;
   return true;
}

Point3D Rectangle::sample(const Point3D& hitPoint, float u1, float u2, Vector3D& normal) {
   normal.set(0, 0, 1);
   return Point3D(u1 * 2.0 - 1.0, u2 * 2.0 - 1.0, 0.0);
}

double Rectangle::pdf(const ShadeRecord& sr) const {
   return 0.25;
}

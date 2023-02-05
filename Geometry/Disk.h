#ifndef _DISK_H_
#define _DISK_H_

#include "GeometryObject.h"

/**
 * Hash values:
 *    - material {} Material hash
 */
class Disk : public GeometryObject {

public:
   Disk();
   virtual ~Disk();

   virtual void setHash(Hash* hash);
   virtual bool hit(const Ray& ray, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray) const;

   virtual Point3D sample(const Point3D& hitPoint, float u1, float u2, Vector3D& normal);
   virtual double pdf(const ShadeRecord& sr) const;
};

#endif

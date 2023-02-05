#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_

#include "GeometryObject.h"
#include "Math/Point3D.h"
#include "Math/Vector3D.h"

class Sampler;

class Rectangle : public GeometryObject {

public:
   Rectangle();
   ~Rectangle();

   virtual bool hit(const Ray& ray, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray) const;
   virtual void setHash(Hash* hash);

   virtual Point3D sample(const Point3D& hitPoint, float u1, float u2, Vector3D& normal);
   virtual double pdf(const ShadeRecord& sr) const;
};

#endif

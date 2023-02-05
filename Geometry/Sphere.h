#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "GeometryObject.h"
#include "Math/Point3D.h"

/**
 * Creates a sphere or a part sphere.
 *
 * Hash values:
 *    - thetaRange [ min max ] - Optional Latitude range to include. Array of 2 daubles from 0 to 180. 0 is at the top of the sphere.
 *    - phiRange [ min max ] - Optional Longitude range to include. Array of 2 doubles from 0 to 360. 0 is in the positive Z axis.
 */
class Sphere : public GeometryObject {

public:
   Sphere();
   virtual ~Sphere();

   virtual bool hit(const Ray& ray, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray) const;
   virtual void setHash(Hash* hash);

   virtual Point3D sample(const Point3D& hitPoint, float u1, float u2, Vector3D& normal);
   virtual double pdf(const ShadeRecord& sr) const;

private:
   bool partCheck(const Ray& ray, double t) const;
   void setTangents(ShadeRecord& sr) const;

   bool thetaRange, phiRange;
   double cosThetaMin, cosThetaMax;
   double phiMin, phiMax;
   float* samples;
   int nSamples, idx;
};

#endif

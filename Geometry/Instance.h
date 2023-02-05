#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include "GeometryObject.h"
#include "Math/Matrix.h"

/**
 * Possible transformations:
 *
 * <ul>
 * <li>translate [ x y z ]
 * <li>scale [ x y z ]
 * <li>rotate [ x y z]
 * </ul>
 */
class Instance : public GeometryObject {

public:
   Instance(GeometryObject* obj);
   virtual ~Instance();

   void setPosition(const Point3D& position);
   void translate(double x, double y, double z);
   void scale(double x, double y, double z);
   void rotate(double x, double y, double z);
   void computeBBox();
   void reset();

   /**
    * Hash values:
    * <ul>
    * <li>object - The object being transformed. This is an style hash with the key object set to the object type.
    * <li>transforms - Array of transformations
    * </ul>
    */
   virtual void setHash(Hash* hash);
   virtual bool hit(const Ray& ray, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray) const;

   virtual void setMaterial(Material* m);

   virtual Point3D sample(const Point3D& hitPoint, float u1, float u2, Vector3D& normal);
   virtual double pdf(const ShadeRecord& sr) const;

private:
   GeometryObject* object;
   Matrix invMatrix;
   Matrix fwdMatrix;
};

inline void Instance::setPosition(const Point3D& position) {
   fwdMatrix.setPosition(position);
   invMatrix.setPosition(-position);
}

inline void Instance::reset() {
   fwdMatrix.setIdentity();
   invMatrix.setIdentity();
}

#endif

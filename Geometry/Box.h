#ifndef _BOX_H_
#define _BOX_H_

#include "GeometryObject.h"

/**
 * Creates a axis aligned box.
 *
 * Hash values:
 * <ul>
 * <li>p0 - Minimum corner. Array of doubles of the form [ x y z ]
 * <li>p1 - Maximum corner. Array of doubles of the form [ x y z ]
 * </ul>
 */
class Box : public GeometryObject {

public:
   Box();
   virtual ~Box() {}

   virtual void setHash(Hash* hash);
   virtual bool hit(const Ray& ray, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray) const;

private:
   void getNormal(Vector3D& n, const int face) const;

   Point3D p0;
   Point3D p1;
};

#endif

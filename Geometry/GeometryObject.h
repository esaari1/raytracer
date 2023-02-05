#ifndef _GEOMETRY_OBJECT_
#define _GEOMETRY_OBJECT_

#include "Math/Ray.h"
#include "Utility/Color.h"
#include "Utility/ShadeRecord.h"
#include "Math/Point3D.h"
#include "BBox.h"

class Hash;
class Material;

class GeometryObject {

public:
   GeometryObject();
   virtual ~GeometryObject();

   virtual void setHash(Hash* hash) = 0;
   virtual bool hit(const Ray& ray, ShadeRecord& sr) const = 0;
   virtual bool shadowHit(const Ray& ray) const = 0;

   Material* getMaterial() const { return material; }
   virtual void setMaterial(Material* m) { material = m; }

   virtual bool isCompound() { return false; }
   string getName() const { return name; }
   void setName(string n) { name = n; }

   virtual Point3D sample(const Point3D& hitPoint, float u1, float u2, Vector3D& normal);
   virtual double pdf(const ShadeRecord& sr) const;

   static const double epsilon;
   bool doDelete;
   bool ignoreShadow;
   BBox bbox;

protected:
   mutable Material* material;
   string name;
};

#endif

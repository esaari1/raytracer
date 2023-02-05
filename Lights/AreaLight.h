#ifndef _AREA_LIGHT_H_
#define _AREA_LIGHT_H_

#include "Light.h"
#include "Math/Point3D.h"
#include "Math/Vector3D.h"

class GeometryObject;

class AreaLight : public Light {

public:
   AreaLight();
   virtual ~AreaLight();

   virtual void setHash(Hash* hash);
   virtual bool inShadow(const Ray& ray, const ShadeRecord& sr);

   virtual Color L(const ShadeRecord& sr, const Vector3D& wi, const Vector3D& normal) const;
   virtual Color Sample_L(ShadeRecord& sr, float u1, float u2, Vector3D& lightDir, float& pdf) const;
   virtual bool isDelta() const { return false; }
   virtual float* getSamples();

private:
   Material* material;
   GeometryObject* object;
   float* samples;
};

#endif

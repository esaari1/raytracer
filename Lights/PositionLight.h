#ifndef _POSITION_H_
#define _POSITION_H_

#include "Light.h"

class PositionLight : public Light {

public:
   PositionLight(float halfDistance = 30.0);
   PositionLight(const Point3D& loc, float halfDistance = 30.0);
   virtual ~PositionLight();

   virtual bool inShadow(const Ray& ray, const ShadeRecord& sr);
   virtual void setHash(Hash* hash);
   
   virtual Color Sample_L(ShadeRecord& sr, float u1, float u2, Vector3D& lightDir, float& pdf) const;
   virtual bool isDelta() const { return true; }
   virtual float* getSamples();

private:
   Color color;
   Point3D location;
   float attenuationPower;
   float* samples;
};

#endif

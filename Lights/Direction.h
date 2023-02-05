#ifndef _DIRECTION_H_
#define _DIRECTION_H_

#include "Light.h"
#include "Utility/Color.h"
#include "Math/Vector3D.h"

class Direction : public Light {

public:
   Direction();
   ~Direction();

   virtual void setHash(Hash* hash);
   virtual bool inShadow(const Ray& ray, const ShadeRecord& sr);

   virtual Color Sample_L(ShadeRecord& sr, float u1, float u2, Vector3D& lightDir, float& pdf) const;
   virtual bool isDelta() const { return true; }
   virtual float* getSamples();

private:
   Color color;
   Vector3D direction;
   int maxDistance;
   float* samples;
};

#endif

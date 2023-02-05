#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Math/Vector3D.h"
#include "Utility/Color.h"
#include "Math/Ray.h"
#include "Utility/ShadeRecord.h"

class Hash;

class Light {

public:
   Light() { numLightSamples = 1; }
   virtual ~Light() {}

   virtual void setHash(Hash* hash) = 0;
   virtual bool inShadow(const Ray& ray, const ShadeRecord& sr) { return false; }

   virtual Color L(const ShadeRecord& sr, const Vector3D& wi, const Vector3D& normal) const { return BLACK; }

   int getNumLightSamples() const { return numLightSamples; }
   virtual float* getSamples() = 0;
   
   virtual Color Sample_L(ShadeRecord& sr, float u1, float u2, Vector3D& lightDir, float& pdf) const = 0;
   virtual Color Le(const ShadeRecord& sr) const { return BLACK; }
   virtual bool isDelta() const = 0;

protected:
   int numLightSamples;
};

#endif

#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include "Light.h"
#include "Math/Vector3D.h"

class Material;
class SamplerNew;

class Environment : public Light {

public:
   Environment();
   ~Environment();

   virtual void setHash(Hash* hash);
   Vector3D getLightDirection(const Vector3D& normal, float u1, float u2) const;
   virtual bool inShadow(const Ray& ray, const ShadeRecord& sr);

   virtual Color Sample_L(ShadeRecord& sr, float u1, float u2, Vector3D& lightDir, float& pdf) const;
   virtual Color Le(const ShadeRecord& sr) const;
   virtual bool isDelta() const { return false; }
   float* getSamples();

private:
   Material* material;
   float* samples;
};

#endif

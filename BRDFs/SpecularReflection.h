#ifndef __SPECULAR_REFLECTION_H__
#define __SPECULAR_REFLECTION_H__

#include "BRDF.h"
#include "Math/Vector3D.h"

class Fresnel;

class SpecularReflection: public BRDF 
{

public:
   SpecularReflection();
   ~SpecularReflection() {}

   virtual Color sample_f(const ShadeRecord& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;
   void setColor(const Color& c) { color = c; }
   
private:
   Color color;
   Fresnel* fresnel;
};

#endif

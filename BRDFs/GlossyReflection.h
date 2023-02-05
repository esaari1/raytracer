#ifndef _GLOSSY_REFLECTION_H_
#define _GLOSSY_REFLECTION_H_

#include "BRDF.h"

class GlossyReflection : public BRDF {

public:
   GlossyReflection(uint32_t ns);
   virtual ~GlossyReflection();

   virtual Color sample_f(const ShadeRecord& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;

   void setExp(const float e) { exp = e; }
   void setColor(const Color& c) { color = c; }

private:
   Color color;
   float exp;
   int nSamples;
   float* samples;
   mutable int idx;
};

#endif

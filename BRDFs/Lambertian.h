#ifndef __LAMBERTIAN__
#define __LAMBERTIAN__

#include "BRDF.h"

class Texture;

class Lambertian : public BRDF {

public:
   Lambertian();
   virtual ~Lambertian() {}

   virtual Color f(const ShadeRecord& sr, const Vector3D& wo, const Vector3D& wi) const;
   virtual Color rho(const ShadeRecord& sr, const Vector3D& wo) const;

   void setColor(float r, float g, float b);
   void setColor(const Color& c) { color = c; }

private:
   Color color;
};

#endif

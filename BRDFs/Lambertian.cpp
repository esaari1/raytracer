#include "Lambertian.h"
#include "Math/Maths.h"
#include "Textures/Texture.h"

Lambertian::Lambertian() : BRDF(BxDFType(DIFFUSE)), color(1, 1, 1) {
}

void Lambertian::setColor(float r, float g, float b) {
   color.set(r, g, b);
}

Color Lambertian::f(const ShadeRecord& sr, const Vector3D& wo, const Vector3D& wi) const {
   if(texture != NULL) {
      Color tc = texture->getColor(sr);
      return tc * color * INV_PI;
   }
   return color * INV_PI;
}

Color Lambertian::rho(const ShadeRecord& sr, const Vector3D& wo) const {
   if(texture != NULL) {
      return texture->getColor(sr) * color;
   }
   return color;
}

#include "SpecularHighlight.h"
#include "Textures/Texture.h"
#include <math.h>

SpecularHighlight::SpecularHighlight() : BRDF(BxDFType(GLOSSY)), exp(0), color(1, 1, 1) {
}

Color SpecularHighlight::f(const ShadeRecord& sr, const Vector3D& wo, const Vector3D& wi) const {
   if(texture != NULL) {
      Color c = texture->getColor(sr);
      if(c == BLACK) {
         return BLACK;
      }
   }

   float ndotwi = sr.normal.dot(wi);
   Vector3D r(wi * -1.0 + sr.normal * 2.0 * ndotwi);
   float rdotwo = r.dot(wo);

   if (rdotwo > 0.0) {
      return color * pow(rdotwo, exp);
   }

   return BLACK;
}

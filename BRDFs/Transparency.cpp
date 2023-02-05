#include "Transparency.h"
#include "Math/Maths.h"
#include "Textures/Texture.h"

Transparency::Transparency() : BRDF(BxDFType(TRANSPARENT)), alpha(1.0) {
}

Transparency::~Transparency() {
   if(texture != NULL) {
      delete texture;
   }
}

double Transparency::getAlpha(const ShadeRecord& sr) const {
   if(texture != NULL) {
      return texture->getAlpha(sr);
   }
   return alpha;
}
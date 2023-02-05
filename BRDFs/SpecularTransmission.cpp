#include "SpecularTransmission.h"
#include "Textures/Texture.h"

SpecularTransmission::SpecularTransmission(float ei, float et) :
   BRDF(BxDFType(TRANSMIT | SPECULAR)),
   color(WHITE),
   etai(ei),
   etat(et),
   fresnel(ei, et),
   texture(NULL)
{
}

SpecularTransmission::~SpecularTransmission() {
   if(texture != NULL) {
      delete texture;
   }
}

Color SpecularTransmission::sample_f(const ShadeRecord& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const {
   float ndotwo = sr.normal.dot(wo);
   Vector3D n = sr.normal;

   float eta = etai / etat;
   if(ndotwo < 0) {
      eta = 1.f / eta;
      n = -n;
      ndotwo = -ndotwo;
   }

   float temp = 1.f - (1.f - ndotwo * ndotwo) / (eta * eta);
   if(temp < 0.0) {
      // total internal reflection has occured, so there is no transmission
      return BLACK;
   }
   float cosTheta2 = sqrtf(temp);
   wi = -wo / eta - n * (cosTheta2 - ndotwo / eta);

   pdf = 1.f;
//   float F = fresnel.evaluate(ndotwo);
//   return color * (1.f - F) / fabs(sr.normal.dot(wi));
   if(texture != NULL) {
      return texture->getColor(sr) * color / fabs(sr.normal.dot(wi));
   }
   return color / fabs(sr.normal.dot(wi));
}

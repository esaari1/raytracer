#include "SpecularReflection.h"
#include "Fresnel.h"

SpecularReflection::SpecularReflection() : BRDF(BxDFType(REFLECT | SPECULAR)), color(1.0, 1.0, 1.0) {
   fresnel = new FresnelConductor(1.0, 1.5);
}

Color SpecularReflection::sample_f(const ShadeRecord& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const {
   pdf = 1.f;
   float ndotwo = sr.normal.dot(wo);

   // Set wi = the reflection of wo throught the surface normal
   wi = -wo + sr.normal * 2.0 * ndotwo;
   
   float ndotwi = fabs(sr.normal.dot(wi));
//   return color * fresnel->evaluate(ndotwo) / ndotwi;
   return color / ndotwi;
}

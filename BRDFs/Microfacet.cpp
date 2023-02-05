#include "Microfacet.h"
#include "Fresnel.h"
#include "Math/Maths.h"
#include <algorithm>
#include <math.h>

float Blinn::D(const Vector3D& normal, const Vector3D& wh, float m) const {
   float costhetah = fabs(normal.dot(wh));
   return (exponent+2) * INV_TWOPI * powf(costhetah, exponent);
/*
   float costhetah = normal.dot(wh);
   float D1 = 1.f / (m * m * costhetah*costhetah*costhetah*costhetah);
   
   float a = acos(costhetah);
   float power = tan(a) / m;
   power = -(power * power);
   float D2 = exp(power);
   float D = D1 * D2;
   return D;
//   power = -pow(a / m, 2);
//   return 0.1f * exp(power);
*/
}

Microfacet::Microfacet(Fresnel* f, MicrofacetDistribution* d) : 
   BRDF(BxDFType(REFLECT)), color(1, 1, 1), fresnel(f), distribution(d)
{
}

float Microfacet::G(const ShadeRecord& sr, const Vector3D& wh, const Vector3D& wo, const Vector3D& wi) const {
   float ndoth = fabs(sr.normal.dot(wh));
   float ndoto = fabs(sr.normal.dot(wo));
   float ndoti = fabs(sr.normal.dot(wi));
   float odoth = fabs(wo.dot(wh));
   return min(1.f, min((2.f * ndoth * ndoto / odoth), (2.f * ndoth * ndoti / odoth)));
}

Color CookTorrance::f(const ShadeRecord& sr, const Vector3D& wo, const Vector3D& wi) const {
   float ndoto = fabs(sr.normal.dot(wo));
   float ndoti = fabs(sr.normal.dot(wi));

   if (ndoti == 0.f || ndoto == 0.f) {
      return BLACK;
   }

   Vector3D wh = wi + wo;
   wh.normalize();
   
   float idoth = wi.dot(wh);
   float F = fresnel->evaluate(idoth);

   float D = distribution->D(sr.normal, wh, 0.2);
//   return color * D * G(sr, wh, wo, wi) * F / (4.f * ndoti * ndoto);
   return color * (F / M_PI) * ((D * G(sr, wh, wo, wi)) / (ndoti * ndoto) );
}

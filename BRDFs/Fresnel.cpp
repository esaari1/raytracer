#include "Fresnel.h"
#include "Math/Maths.h"

FresnelDielectric::FresnelDielectric(float ei, float et) : eta_i(ei), eta_t(et) {}

float FresnelDielectric::evaluate(float cosi) const {
   cosi = clamp(cosi, -1.f, 1.f);

   float etai = eta_i, etat = eta_t;
   if(cosi <= 0.f) {
      swap(etai, etat);
   }

   float sint = etai / etat * sqrtf(max(0.f, 1.f - cosi * cosi));
   if(sint >= 1.f) {
      return 1.f;
   }

   float cost = sqrtf(max(0.f, 1.f - sint * sint));
   cosi = fabs(cosi);

   float parallel = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
   float perp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
   return (parallel*parallel + perp*perp) * 0.5f;
}

FresnelConductor::FresnelConductor(float _eta, float _k) : eta(_eta), k(_k) {}

float FresnelConductor::evaluate(float cosi) const {
   cosi = fabs(cosi);
   float tmp = (eta * eta + k * k) * cosi * cosi;
   float parallel = (tmp - (2.f * eta * cosi) + 1) / (tmp + (2.f * eta * cosi) + 1);

   tmp = eta * eta * k * k;
   float perp = (tmp - (2.f * eta * cosi) + cosi * cosi) / (tmp + (2.f * eta * cosi) + cosi * cosi);
   return (parallel + perp) * 0.5f;
}

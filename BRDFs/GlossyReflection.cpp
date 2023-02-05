#include "GlossyReflection.h"
#include "Samplers/Sampler.h"

GlossyReflection::GlossyReflection(uint32_t ns) : BRDF(BxDFType(REFLECT | GLOSSY)), color(WHITE), exp(0), nSamples(ns), idx(0) {
   samples = new float[nSamples * 2];
   Sampler::LatinHyperCube(samples, nSamples, 2);
}

GlossyReflection::~GlossyReflection() {
   delete[] samples;
}

Color GlossyReflection::sample_f(const ShadeRecord& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const {
   static const Vector3D vec(0.00424, 1.0, 0.00764);

   float ndotwo = sr.normal.dot(wo);
   Vector3D w(-wo + sr.normal * 2.0 * ndotwo);

   Vector3D u = vec.cross(w);
   u.normalize();

   Vector3D v = u.cross(w);

   float x = samples[idx];
   float y = samples[idx+1];
   idx = (idx + 2) % (nSamples * 2);
   Vector3D sp = Sampler::mapToHemisphere(x, y);

   wi = u * sp.x + v * sp.y + w * sp.z;

   float ndotwi = sr.normal.dot(wi);
   if(ndotwi < 0.0) {
      wi = u * -sp.x + v * -sp.y + w * sp.z;
   }

   float lobe = pow(w.dot(wi), exp);
   pdf = lobe * ndotwi;

   return color * lobe;

}

#include "AmbientOccluder.h"
#include "Geometry/GeometryManager.h"
#include "Geometry/GeometryObject.h"
#include "Parser/Hash.h"
#include "Storage/Storage.h"
#include "Samplers/Sampler.h"

AmbientOccluder::AmbientOccluder() : Ambient(), minColor() {
}

AmbientOccluder::~AmbientOccluder() {
   delete[] samples;
}

Vector3D AmbientOccluder::getLightDirection(ShadeRecord& sr) {
   Vector3D sp = Sampler::mapToHemisphere(samples[idx], samples[idx+1]);
   idx = (idx + 2) % (nSamples * 2);
   return u * sp.x + v * sp.y + w * sp.z;
   return Vector3D();
}

void AmbientOccluder::setHash(Hash* hash) {
   ls = hash->getDouble("radiance");
   color.set(hash->getValue("color")->getArray());
   minColor.set(hash->getValue("minColor")->getArray());

   nSamples = hash->getInteger("numSamples");
   idx = 0;
   samples = new float[nSamples * 2];
   Sampler::LatinHyperCube(samples, nSamples, 2);
}

bool AmbientOccluder::inShadow(const Ray& ray, const ShadeRecord& sr) {
   if(GeometryManager::instance().getStorage()->shadowHit(ray)) {
      return true;
   }
   return false;
}

Color AmbientOccluder::L(const ShadeRecord& sr) {
   w = sr.normal;
   v = w.cross(0.0072, 1.0, 0.0034);
   v.normalize();
   u = v.cross(w);

   Ray shadowRay;
   shadowRay.origin = sr.hitPoint;

   Vector3D sp = Sampler::mapToHemisphere(samples[idx], samples[idx+1]);
   idx = (idx + 2) % (nSamples * 2);

   shadowRay.direction = u * sp.x + v * sp.y + w * sp.z;

   if(inShadow(shadowRay, sr)) {
      return minColor * color * ls;
   }
   return color * ls;
}

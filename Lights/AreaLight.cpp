#include "AreaLight.h"
#include "Geometry/GeometryObject.h"
#include "Geometry/GeometryManager.h"
#include "Materials/Material.h"
#include "Materials/Emissive.h"
#include "Parser/Hash.h"
#include "Storage/Storage.h"
#include "Samplers/Sampler.h"

AreaLight::AreaLight() : Light(), material(NULL), object(NULL), samples(NULL) {
}

AreaLight::~AreaLight() {
   delete material;
   delete[] samples;
}

void AreaLight::setHash(Hash* hash) {
   material = new Emissive();
   material->setHash(hash);

   if(hash->contains("rectangle")) {
      Hash* rect = hash->getValue("rectangle")->getHash();
      object = GeometryManager::instance().createObject("rectangle", rect, false);
      object->ignoreShadow = true;
   }
   else if(hash->contains("disk")) {
      Hash* disk = hash->getValue("disk")->getHash();
      object = GeometryManager::instance().createObject("disk", disk, false);
      object->ignoreShadow = true;
   }
   else if(hash->contains("sphere")) {
      Hash* sphere = hash->getValue("sphere")->getHash();
      object = GeometryManager::instance().createObject("sphere", sphere);
      object->ignoreShadow = true;
   }

   numLightSamples = hash->getInteger("numLightSamples");
   samples = new float[numLightSamples * 2];
}

bool AreaLight::inShadow(const Ray& ray, const ShadeRecord& sr) {
   const double ts = (sr.samplePoint - ray.origin).dot(ray.direction);

   if(GeometryManager::instance().getStorage()->shadowHit(ray) && (ray.tHit < ts)) {
      return true;
   }
   return false;
}

Color AreaLight::L(const ShadeRecord& sr, const Vector3D& wi, const Vector3D& normal) const {
   const float ndotwi = -normal.dot(wi);
   if(ndotwi > 0.0) {
      return material->getLe(sr);
   }
   return BLACK;
}

Color AreaLight::Sample_L(ShadeRecord& sr, float u1, float u2, Vector3D& lightDir, float& pdf) const {
   Vector3D normal;
   sr.samplePoint = object->sample(sr.hitPoint, u1, u2, normal);
   lightDir = (sr.samplePoint - sr.hitPoint).normalize();
   pdf = object->pdf(sr);
   return L(sr, lightDir, normal);
}

float* AreaLight::getSamples() {
   Sampler::LatinHyperCube(samples, numLightSamples, 2);
   return samples;
}

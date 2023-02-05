#include "Environment.h"
#include "Samplers/Sampler.h"
#include "Parser/Hash.h"
#include "Geometry/GeometryManager.h"
#include "Geometry/GeometryObject.h"
#include "Materials/Emissive.h"
#include "Storage/Storage.h"

const float PDF = 1.0 / 2.0 * M_PI;

Environment::Environment() : Light(), material(new Emissive()), samples(NULL) {
}

Environment::~Environment() {
   delete material;
   delete[] samples;
}

void Environment::setHash(Hash* hash) {
   material->setHash(hash);   
   numLightSamples = hash->getInteger("numLightSamples");   
   samples = new float[numLightSamples * 2];
}

Vector3D Environment::getLightDirection(const Vector3D& n, float u1, float u2) const {
   Vector3D u;

   if(fabs(n.x) > fabs(n.y)) {
      u.set(-n.z, 0.0, n.x);
   }
   else {
      u.set(0.0, n.z, n.y);
   }
   Vector3D v = n.cross(u);

   Vector3D sp = Sampler::mapToHemisphere(u1, u2);
   return u * sp.x + v * sp.y + n * sp.z;
}

bool Environment::inShadow(const Ray& ray, const ShadeRecord& sr) {
   if(GeometryManager::instance().getStorage()->shadowHit(ray)) {
      return true;
   }
   return false;
}

Color Environment::Sample_L(ShadeRecord& sr, float u1, float u2, Vector3D& lightDir, float& pdf) const {
   lightDir = getLightDirection(sr.normal, u1, u2);
   pdf = PDF;
   return material->getLe(sr);
}

Color Environment::Le(const ShadeRecord& sr) const {
   return material->getLe(sr);
}

float* Environment::getSamples() {
   Sampler::LatinHyperCube(samples, numLightSamples, 2);
   return samples;
}

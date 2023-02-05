#include "Direction.h"
#include "Parser/Hash.h"
#include "Geometry/GeometryManager.h"
#include "Geometry/GeometryObject.h"
#include "Storage/Storage.h"
#include <limits>

using namespace std;

Direction::Direction() : Light(), color(), direction(0, 0, -1), maxDistance(numeric_limits<int>::max()) {
   samples = new float[2];
   samples[0] = samples[1] = 0.5;
}

Direction::~Direction() {
   delete[] samples;
}

void Direction::setHash(Hash* hash) {
   color.set(hash->getValue("color")->getArray());
   color *= hash->getDouble("radiance");
   
   if(hash->contains("direction")) {
      direction.set(hash->getValue("direction")->getArray());
      direction.normalize();
   }
   
   if(hash->contains("rotation")) {
      Array* a = hash->getValue("rotation")->getArray();
      Matrix m;
      m.rotateZ(a->at(2)->getDouble());
      m.rotateY(a->at(1)->getDouble());
      m.rotateX(a->at(0)->getDouble());
      
      direction = m * direction;
      direction.normalize();
   }
   
   if(hash->contains("maxDistance")) {
      maxDistance = hash->getInteger("maxDistance");
   }
}

bool Direction::inShadow(const Ray& ray, const ShadeRecord& sr) {
   if(GeometryManager::instance().getStorage()->shadowHit(ray) && (ray.tHit < maxDistance)) {
      return true;
   }
   return false;
}

Color Direction::Sample_L(ShadeRecord& sr, float u1, float u2, Vector3D& lightDir, float& pdf) const {
   lightDir = -direction;
   pdf = 1.0;
   return color;
}

float* Direction::getSamples() {
   return samples;
}

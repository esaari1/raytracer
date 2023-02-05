#include "Tracer.h"
#include "Math/Maths.h"
#include "Geometry/GeometryManager.h"
#include "Storage/Storage.h"
#include "Materials/Material.h"

Tracer::Tracer() : bgColor(), texture(NULL) {
}

Tracer::~Tracer() {
   if(texture != NULL) {
      delete texture;
   }
}

Color Tracer::traceRay(const Ray& ray, bool useBackground) {
   ShadeRecord sr;
   ray.tHit = HUGE_VALUE;
   
   if(GeometryManager::instance().getStorage()->hit(ray, sr)) {
      return integrator.shade(sr, ray, this);
   }
   else {
      if(!useBackground) {
         return BLACK;
      }
      if(texture != NULL) {
         sr.localHitPoint.set(ray.direction.x, ray.direction.y, ray.direction.z);
         return texture->getColor(sr);
      }
      return bgColor;
   }
}

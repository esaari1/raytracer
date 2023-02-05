#include "Compound.h"
#include <math.h>

Compound::Compound() : GeometryObject() {
}

Compound::~Compound() {
   for(unsigned int i = 0; i < objects.size(); i++) {
      delete objects[i];
   }
   objects.clear();
}

void Compound::setHash(Hash* hash) {
   for(CompoundIter it = objects.begin(); it != objects.end(); it++) {
      (*it)->setHash(hash);
   }
}

void Compound::addObject(GeometryObject* obj) {
   objects.push_back(obj);
   bbox.expand(obj->bbox);
}

bool Compound::hit(const Ray& ray, ShadeRecord& sr) const {
   if(!bbox.hit(ray)) {
      return false;
   }

   for(CompoundIter it = objects.begin(); it != objects.end(); it++) {
      if((*it)->hit(ray, sr)) {
         return true;
      }
   }
   return false;
}

bool Compound::shadowHit(const Ray& ray) const {
   if(!bbox.hit(ray)) {
      return false;
   }

   for(CompoundIter it = objects.begin(); it != objects.end(); it++) {
      if((*it)->shadowHit(ray)) {
         return true;
      }
   }

   return false;
}

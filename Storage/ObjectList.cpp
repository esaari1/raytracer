#include "ObjectList.h"
#include "Math/Maths.h"

bool ObjectList::hit(const Ray& ray, ShadeRecord& sr) const {
   bool hit = false;
   for(GeomIter it = objs.begin(); it != objs.end(); ++it) {
      if((*it)->hit(ray, sr)) {
         hit = true;
      }
   }

   return hit;
}

bool ObjectList::shadowHit(const Ray& ray) const {
   for(GeomIter it = objs.begin(); it != objs.end(); it++) {
      if(!(*it)->ignoreShadow && (*it)->shadowHit(ray)) {
         return true;
      }
   }

   return false;
}

#include "Storage.h"
#include "Geometry/GeometryObject.h"
#include "Geometry/Compound/Compound.h"
#include <algorithm>

Storage::Storage() : objs() {}

void Storage::addObject(GeometryObject* obj) {
   if (obj->isCompound()) {
      const vector<GeometryObject*>& objects = ((Compound*)obj)->getObjects();
      for(CompoundIter it = objects.begin(); it != objects.end(); it++) {
         addObject(*it);
      }
   }
   else {
      objs.push_back(obj);
      bbox.expand(obj->bbox);
   }
}

GeometryObject* Storage::getObject(const string& name) {
   for(GeomIter it = objs.begin(); it != objs.end(); ++it) {
      if((*it)->getName() == name) {
         return *it;
      }
   }
   return NULL;
}

void Storage::removeObject(GeometryObject* obj) {
   if (obj->isCompound()) {
      const vector<GeometryObject*>& objects = ((Compound*)obj)->getObjects();
      for(CompoundIter it = objects.begin(); it != objects.end(); it++) {
         removeObject(*it);
      }
   }
   else {
      vector<GeometryObject*>::iterator it = find(objs.begin(), objs.end(), obj);
      objs.erase(it);
   }
}

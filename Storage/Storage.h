#ifndef _STORAGE_H_
#define _STORAGE_H_

#include "Geometry/GeometryObject.h"
#include <vector>

using namespace std;

typedef vector<GeometryObject*>::const_iterator GeomIter;

class Storage : public GeometryObject {

public:
   Storage();
   virtual ~Storage() {}
   virtual void setup() = 0;

   void addObject(GeometryObject* obj);
   GeometryObject* getObject(const string& name);
   void removeObject(GeometryObject* obj);

protected:
   vector<GeometryObject*> objs;
};

#endif

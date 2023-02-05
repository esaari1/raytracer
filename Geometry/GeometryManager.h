#ifndef _GEOMETRY_MANAGER_H_
#define _GEOMETRY_MANAGER_H_

#include "Math/Point3D.h"
#include "Math/Vector3D.h"
#include <map>
#include <memory>
#include <string>

using namespace std;

class GeometryObject;
class Hash;
class Storage;

class GeometryManager {

public:
   static GeometryManager& instance();
   ~GeometryManager();

   void loadObjects(string fname);

   GeometryObject* createObject(string type, Hash* hash, bool addToList = true);

   Storage* getStorage() { return storage; }
   void setStorageConfig(Hash* h);

   /**
    * Create a sphere wrapped in an Instance object. Instance is used to center sphere to
    * (cx, cy, cz) and scales it by r.
    */
   static GeometryObject* createSphere(double cx, double cy, double cz, double r);
   static GeometryObject* createRectangle(const Vector3D& translate, double scalex, double scaley, const Vector3D& rotation);
   static GeometryObject* createDisk(const Vector3D& translate, double scalex, double scaley, const Vector3D& rotation);

private:
   GeometryManager();

   static auto_ptr<GeometryManager> s_instance;
   Storage* storage;
};

#endif

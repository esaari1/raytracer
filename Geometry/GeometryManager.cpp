#include <fstream>
#include "GeometryManager.h"
#include "GeometryObject.h"
#include "Sphere.h"
#include "Plane.h"
#include "Rectangle.h"
#include "Cylinder.h"
#include "Disk.h"
#include "Torus.h"
#include "Box.h"
#include "Annulus.h"
#include "StarBox.h"
#include "Compound/Wedge.h"
#include "Compound/WedgeRing.h"
#include "Compound/MarbleCheckerBoard.h"
#include "Instance.h"
#include "Parser/Parser.h"
#include "Materials/Matte.h"
#include "Mesh/MeshManager.h"
#include "Mesh/Mesh.h"
#include "Mesh/GeoSphere.h"
#include "Storage/Grid.h"
#include "Storage/KdTree.h"
#include "Storage/ObjectList.h"

typedef map<string, GeometryObject*>::const_iterator GeometryIter;

auto_ptr<GeometryManager> GeometryManager::s_instance;

GeometryManager& GeometryManager::instance() {
   if(s_instance.get() == 0) {
      s_instance.reset(new GeometryManager());
   }
   return *s_instance;
}

GeometryManager::GeometryManager() : storage(new ObjectList()) {
}

GeometryManager::~GeometryManager() {
   delete storage;
}

void GeometryManager::loadObjects(string fname) {
   std::ifstream fp(fname.c_str());
   Tokenizer tok(&fp);
   Parser parser(&tok);

   while(tok.nextToken() != Tokenizer::TokenEnd) {
      if(tok.getTokenType() != Tokenizer::TokenName) {
         return ;
      }

      string type = tok.getStringValue();
      createObject(type, parser.readValue()->getHash());
   }

   fp.close();
   storage->setup();
}

GeometryObject* GeometryManager::createObject(string type, Hash* hash, bool addToList) {
   GeometryObject* obj;
   string name = hash->getString("name");

   if(type == "sphere") {
      obj = new Sphere();
   }
   else if(type == "plane") {
      obj = new Plane();
   }
   else if(type == "rectangle") {
      obj = new Rectangle();
   }
   else if(type == "cylinder") {
      obj = new Cylinder();
   }
   else if(type == "disk") {
      obj = new Disk();
   }
   else if(type == "torus") {
      obj = new Torus();
   }
   else if(type == "box") {
      obj = new Box();
   }
   else if(type == "annulus") {
      obj = new Annulus();
   }
   else if(type == "wedge") {
      obj = new Wedge();
   }
   else if(type == "wedgeRing") {
      obj = new WedgeRing();
   }
   else if(type == "marbleCheckerBoard") {
      obj = new MarbleCheckerBoard();
   }
   else if(type == "geo") {
      obj = new GeoSphere();
   }
   else if(type == "triangles") {
      obj = new Mesh();
   }
   else if(type == "starBox") {
      StarBox sb;
      sb.setHash(hash);
      sb.createStars();
      return NULL;
   }
   else if(type == "mesh") {
      name = hash->getString("name");
      obj = MeshManager::instance().getMesh(name);
      obj->setName(name);
   }
   else {
      return NULL;
   }

   obj->setHash(hash);

   if(hash->contains("transforms")) {
      obj = new Instance(obj);
      obj->setHash(hash);
   }

   if(addToList) {
      storage->addObject(obj);
   }

   return obj;
}

void GeometryManager::setStorageConfig(Hash* h) {
   string type = h->getString("type");

   // Since storage is set up as an object list in the constructor
   // we only need to delete it if the storage type changes
   if(type == "grid") {
      delete storage;
      storage = new Grid();
   }
   else if(type == "kdtree") {
      delete storage;
      storage = new KdTree();
   }
   storage->setHash(h);
}

GeometryObject* GeometryManager::createSphere(double cx, double cy, double cz, double r) {
   Instance* instance = new Instance(new Sphere());
   instance->scale(r, r, r);
   instance->translate(cx, cy, cz);
   instance->computeBBox();
   return instance;
}

GeometryObject* GeometryManager::createRectangle(const Vector3D& translate, double scalex, double scaley, const Vector3D& rotation) {
   Instance* instance = new Instance(new Rectangle());
   instance->scale(scalex, scaley, 1);
   instance->rotate(rotation.x, rotation.y, rotation.z);
   instance->translate(translate.x, translate.y, translate.z);
   instance->computeBBox();
   return instance;
}

GeometryObject* GeometryManager::createDisk(const Vector3D& translate, double scalex, double scaley, const Vector3D& rotation) {
   Instance* instance = new Instance(new Disk());
   instance->scale(scalex, scaley, 1);
   instance->rotate(rotation.x, rotation.y, rotation.z);
   instance->translate(translate.x, translate.y, translate.z);
   instance->computeBBox();
   return instance;
}

#include "StarBox.h"
#include "Geometry/GeometryManager.h"
#include "Parser/Hash.h"
#include "Textures/Texture.h"
#include "Storage/Storage.h"

StarBox::StarBox() :
   halfSize(1),
   yzNegX(NULL),
   yzPosX(NULL),
   xyNegZ(NULL),
   xyPosZ(NULL),
   xzNegY(NULL),
   xzPosY(NULL)
{
}

void StarBox::setHash(Hash* h) {
   halfSize = h->getInteger("halfSize");

   if(h->contains("yzNegX")) {
      yzNegX = Texture::createTexture(h->getValue("yzNegX")->getHash());
   }
   if(h->contains("yzPosX")) {
      yzPosX = Texture::createTexture(h->getValue("yzPosX")->getHash());
   }
   if(h->contains("xyNegZ")) {
      xyNegZ = Texture::createTexture(h->getValue("xyNegZ")->getHash());
   }
   if(h->contains("xyPosZ")) {
      xyPosZ = Texture::createTexture(h->getValue("xyPosZ")->getHash());
   }
   if(h->contains("xzNegY")) {
      xzNegY = Texture::createTexture(h->getValue("xzNegY")->getHash());
   }
   if(h->contains("xzPosY")) {
      xzPosY = Texture::createTexture(h->getValue("xzPosY")->getHash());
   }
}

void StarBox::createStars() {
   if(yzNegX != NULL) createBoxSide(yzNegX, Vector3D(-halfSize, 0, 0), Vector3D(0, 90, 0));
   if(yzPosX != NULL) createBoxSide(yzPosX, Vector3D(halfSize, 0, 0), Vector3D(0, -90, 0));
   if(xyNegZ != NULL) createBoxSide(xyNegZ, Vector3D(0, 0, -halfSize), Vector3D(0, 0, 0));
   if(xyPosZ != NULL) createBoxSide(xyPosZ, Vector3D(0, 0, halfSize), Vector3D(0, 180, 0));
   if(xzNegY != NULL) createBoxSide(xzNegY, Vector3D(0, -halfSize, 0), Vector3D(-90, 0, 0));
   if(xzPosY != NULL) createBoxSide(xzPosY, Vector3D(0, halfSize, 0), Vector3D(90, 0, 0));
}

void StarBox::createBoxSide(Texture* texture, const Vector3D& translate, const Vector3D& rotate) {
   Emissive* em = new Emissive();
   em->setTexture(texture);

   GeometryObject* rect = GeometryManager::createRectangle(translate, halfSize, halfSize, rotate);
   rect->setMaterial(em);
   rect->ignoreShadow = true;
   GeometryManager::instance().getStorage()->addObject(rect);
}

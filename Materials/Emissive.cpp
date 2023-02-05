#include "Emissive.h"
#include "Parser/Hash.h"
#include "Textures/Texture.h"

Emissive::Emissive() : texture(NULL) {
   ls = 1.0;
}

Emissive::~Emissive() {
   if(texture != NULL) {
      delete texture;
   }
}

void Emissive::setHash(Hash* hash) {
   if(hash->contains("radiance")) {
      ls = hash->getDouble("radiance");
   }
   if(hash->contains("color")) {
      color.set(hash->getValue("color")->getArray());
   }
   
   if(hash->contains("texture")) {
      texture = Texture::createTexture(hash->getValue("texture")->getHash());
   }
}

Color Emissive::getLe(const ShadeRecord& sr) const {
   if(texture != NULL) {
      return texture->getColor(sr) * ls;
   }
   return color * ls;
}

void Emissive::setTexture(Texture* tex) {
   texture = tex;
}

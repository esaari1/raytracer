#include "Texture.h"
#include "Parser/Hash.h"
#include "PlaneChecker.h"
#include "MarbleChecker.h"
#include "NoiseTexture.h"
#include "SkyTexture.h"
#include "WoodTexture.h"
#include "GraniteTexture.h"
#include "MarbleTexture.h"
#include "ImageTexture.h"
#include "TInstance.h"

Texture* Texture::createTexture(Hash* hash) {
   string type = hash->getString("type");
   Texture* texture;

   if(type == "planeChecker") {
      texture = new PlaneChecker();
   }
   else if(type == "marbleChecker") {
      texture = new MarbleChecker();
   }
   else if(type == "noise") {
      texture = new NoiseTexture();
   }
   else if(type == "sky") {
      texture = new SkyTexture();
   }
   else if(type == "wood") {
      texture = new WoodTexture();
   }
   else if(type == "granite") {
      texture = new GraniteTexture();
   }
   else if(type == "marble") {
      texture = new MarbleTexture();
   }
   else if(type == "image") {
      texture = new ImageTexture();
   }
   else {
      printf("Unknown texture type %s\n", type.c_str());
      exit(1);
   }
   texture->setHash(hash);
   
   if(hash->contains("transforms")) {
      texture = new TInstance(texture);
      texture->setHash(hash);   
   }

   return texture;
}

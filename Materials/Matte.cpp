#include "Matte.h"
#include "Math/Maths.h"
#include "Lights/LightManager.h"
#include "BRDFs/Lambertian.h"
#include "Textures/Texture.h"
#include "Textures/ImageTexture.h"
#include "Parser/Hash.h"
#include "Lights/Light.h"
#include "Samplers/Sampler.h"

Matte::Matte() : texture(NULL) {
}

Matte::~Matte() {
   if(texture != NULL) {
      delete texture;
   }
}

void Matte::setHash(Hash* hash) {
   Lambertian* diffuseBRDF = new Lambertian();

   if(hash->contains("texture")) {
      texture = Texture::createTexture(hash->getValue("texture")->getHash());
      ambientBRDF->setTexture(texture, false);
      diffuseBRDF->setTexture(texture, false);
   }
   else if(hash->contains("textureFile")) {
      texture = new ImageTexture();
      ((ImageTexture*) texture)->setTextureFile(hash->getString("textureFile"));
      ambientBRDF->setTexture(texture, false);
      diffuseBRDF->setTexture(texture, false);
   }

   if(hash->contains("color")) {
      diffuseBRDF->setColor(hash->getValue("color")->getArray());
   }

   if(hash->contains("ambientColor")) {
      ambientBRDF->setColor(Color(hash->getValue("ambientColor")->getArray()));
   }

   bsdf.addBRDF(diffuseBRDF);
}

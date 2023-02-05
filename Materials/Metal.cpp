#include "Metal.h"
#include "Parser/Hash.h"
#include "BRDFs/Microfacet.h"
#include "BRDFs/Fresnel.h"
#include "BRDFs/Lambertian.h"
#include "Textures/ImageTexture.h"
#include "Lights/LightManager.h"
#include "Lights/Light.h"

Metal::Metal() : texture(NULL) {
}

Metal::~Metal() {
   if(texture != NULL) {
      delete texture;
   }
}

void Metal::setHash(Hash* hash) {
//   double kd = hash->getDouble("kd", 1.0);
   
   float eta = hash->getDouble("eta", 1.9);
   float k = hash->getDouble("k", 4.5);

   Lambertian* diffuseBRDF = new Lambertian();
   MicrofacetDistribution *md = new Blinn();
   Fresnel *frMf = new FresnelConductor(eta, k);
   Microfacet* microfacet = new CookTorrance(frMf, md);
   
   if(hash->contains("texture")) {
      texture = Texture::createTexture(hash->getValue("texture")->getHash());
      ambientBRDF->setTexture(texture, false);
      diffuseBRDF->setTexture(texture, false);
      microfacet->setTexture(texture, false);
   }
   else if(hash->contains("textureFile")) {
      texture = new ImageTexture();
      ((ImageTexture*) texture)->setTextureFile(hash->getString("textureFile"));
      ambientBRDF->setTexture(texture, false);
      diffuseBRDF->setTexture(texture, false);
      microfacet->setTexture(texture, false);
   }
   
   if(hash->contains("color")) {
      Array* a = hash->getValue("color")->getArray();
      ambientBRDF->setColor(Color(a));
      diffuseBRDF->setColor(Color(a));
      microfacet->setColor(Color(a));
   }
   
   if(hash->contains("ambientColor")) {
      ambientBRDF->setColor(Color(hash->getValue("ambientColor")->getArray()));
   }
   
   bsdf.addBRDF(diffuseBRDF);
   bsdf.addBRDF(microfacet);
}

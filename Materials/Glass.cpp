#include "Glass.h"
#include "BRDFs/Lambertian.h"
#include "BRDFs/SpecularHighlight.h"
#include "BRDFs/SpecularReflection.h"
#include "BRDFs/SpecularTransmission.h"
#include "Parser/Hash.h"

Glass::Glass() { }

void Glass::setHash(Hash* hash) {
   Lambertian* diffuse = new Lambertian();   
   if(hash->contains("color")) {
      diffuse->setColor(hash->getValue("color")->getArray());
   }
   bsdf.addBRDF(diffuse);

   SpecularHighlight* highlight = new SpecularHighlight();
   highlight->setColor(WHITE);
   highlight->setExp(2000.0);
   bsdf.addBRDF(highlight);

   float T = hash->getDouble("transparency", 0.0);
   float R = 1.0 - T;
   
   if(R > 0.0) {
      SpecularReflection* reflection = new SpecularReflection();
      reflection->setColor(Color(R, R, R));
      bsdf.addBRDF(reflection);
   }

   if(T > 0.0) {
      SpecularTransmission* transmission = new SpecularTransmission(1.5, 1.0);
      transmission->setColor(Color(T, T, T));
      bsdf.addBRDF(transmission);
   }
}

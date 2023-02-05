#include "Reflective.h"
#include "BRDFs/SpecularReflection.h"
#include "Tracer/Tracer.h"
#include "Parser/Hash.h"

Reflective::Reflective() : psBRDF(new SpecularReflection) {
}

Reflective::~Reflective() {
   delete psBRDF;
}

void Reflective::setHash(Hash* hash) {
   Phong::setHash(hash);
   psBRDF->setColor(hash->getValue("reflectColor")->getArray());   
   bsdf.addBRDF(psBRDF);
}

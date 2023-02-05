#ifndef _REFLECTIVE_H_
#define _REFLECTIVE_H_

#include "Phong.h"

class SpecularReflection;
class Hash;

/**
 * Hash values:
 *    - kr <i>value</i> - reflective contribution
 *    - reflectColor [ r g b ] - reflection color
 *    - ka <i>value</i> - ambient contribution
 *    - kd <i>value</i> - diffuse contribution
 *    - ks <i>value</i> - specular contribution
 *    - exp <i>value</i> - specular exponent
 *    - color [ r g b ] - Optional material color
 */
class Reflective : public Phong {

public:
   Reflective();
   virtual ~Reflective();

   virtual void setHash(Hash* hash);

private:
   SpecularReflection* psBRDF;
};

#endif

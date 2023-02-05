#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Utility/Color.h"
#include "Utility/ShadeRecord.h"
#include "Math/Ray.h"
#include "BRDFs/BSDF.h"

class Hash;
class Texture;
class BSDF;
class Lambertian;
class Light;

/**
 * Hash values:
 *    - type <i>string</i> - Defines the type of material to use. (phong, reflective, matte)
 *
 * Material classes:
 *    - Matte
 *    - Phong
 *    - Reflective
 */
class Material {

public:
   Material();
   virtual ~Material();

   static Material* createMaterial(Hash* hash);

   virtual void setHash(Hash* hash) = 0;
   virtual Color getLe(const ShadeRecord& sr) const { return BLACK; }

   void applyNormalMap(ShadeRecord& sr);
   virtual bool shadowHit(ShadeRecord& sr) { return true; }
   
   Lambertian* ambientBRDF;
   BSDF bsdf;

protected:
   Texture* normalMap;
};

#endif

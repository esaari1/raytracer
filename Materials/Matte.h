#ifndef _MATTE_H_
#define _MATTE_H_

#include "Material.h"

class Texture;

/**
 * Hash values:
 * <ul>
 * <li>color [ r g b ] - Optional material color
 * <li>ambientColor [ r g b ] - Optional material ambient color
 * <li>texture {} - Optional texture hash. See Texture.
 * </ul>
 * If neither color or texture is specified, material defaults to black.
 */
class Matte : public Material {

public:
   Matte();
   virtual ~Matte();

   virtual void setHash(Hash* hash);
   
private:
   Texture* texture;
};

#endif

#ifndef raytracer_AlphaBlend_h
#define raytracer_AlphaBlend_h

#include "Material.h"

class Texture;

/**
 * Hash values:
 *
 * texture - Texture definition as a Hash
 * alphaThreshold - double, default is 0.5
 * kd (optional) - double, applied to diffuse color. Default color is WHITE
 * ka (optional) - double, applied to ambient color. Default color is WHITE
 */
class AlphaBlend : public Material {

public:
   AlphaBlend() {}
   virtual ~AlphaBlend();

   virtual void setHash(Hash* hash);
   virtual bool shadowHit(ShadeRecord& sr);
   
private:
   Texture* texture;
   float alphaThreshold;
};

#endif

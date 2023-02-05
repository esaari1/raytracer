#ifndef _AMBIENT_OCCLUDER_H_
#define _AMBIENT_OCCLUDER_H_

#include "Ambient.h"

class AmbientOccluder : public Ambient {
   
public:
   AmbientOccluder();
   virtual ~AmbientOccluder();

   virtual Vector3D getLightDirection(ShadeRecord& sr);
   virtual void setHash(Hash* hash);
   virtual bool inShadow(const Ray& ray, const ShadeRecord& sr);
   
   virtual Color L(const ShadeRecord& sr);
   
private:
   Color minColor;
   Vector3D u, v, w;
   uint32_t nSamples, idx;
   float* samples;
};

#endif

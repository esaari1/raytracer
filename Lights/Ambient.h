#ifndef _AMBIENT_H_
#define _AMBIENT_H_

#include "Light.h"

class Ambient : public Light {
   
public:
   Ambient();
   virtual ~Ambient();
   
   virtual void setHash(Hash* hash);
   virtual Color L(const ShadeRecord& sr);

   virtual Color Sample_L(ShadeRecord& sr, float u1, float u2, Vector3D& lightDir, float& pdf) const;
   virtual bool isDelta() const { return true; }
   virtual float* getSamples();
   
protected:
   float ls;
   Color color;
   float* samples;
};

#endif

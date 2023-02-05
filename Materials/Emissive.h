#ifndef _EMISSIVE_H_
#define _EMISSIVE_H_

#include "Material.h"

class Texture;

class Emissive : public Material {

public:
   Emissive();
   virtual ~Emissive();

   virtual void setHash(Hash* hash);
   virtual Color getLe(const ShadeRecord& sr) const;

   void setTexture(Texture* tex);

private:
   float ls;
   Color color;
   Texture* texture;
};

#endif

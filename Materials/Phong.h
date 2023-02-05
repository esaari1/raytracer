#ifndef _PHONG_H_
#define _PHONG_H_

#include "Material.h"

class Texture;

class Phong : public Material {

public:
   Phong();
   virtual ~Phong();

   virtual void setHash(Hash* hash);
   
private:
   Texture* texture;
};

#endif

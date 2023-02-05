#ifndef _MARBLE_H_
#define _MARBLE_H_

#include "Material.h"

class Texture;

class Marble : public Material {

public:
   Marble() {}
   ~Marble();
   
   virtual void setHash(Hash* hash);
   
private:
   Texture* texture;
};

#endif

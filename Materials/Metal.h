#ifndef raytracer_Metal_h
#define raytracer_Metal_h

#include "Material.h"

class Texture;

class Metal : public Material {

public:
   Metal();
   virtual ~Metal();
   
   virtual void setHash(Hash* hash);

private:
   Texture* texture;
};

#endif

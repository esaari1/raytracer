#ifndef _GLASS_H_
#define _GLASS_H_

#include "Material.h"

class Glass : public Material {

public:
   Glass();
   virtual void setHash(Hash* hash);
};

#endif

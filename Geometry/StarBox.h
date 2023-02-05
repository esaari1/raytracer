#ifndef _STARBOX_H_
#define _STARBOX_H_

#include "Materials/Emissive.h"
#include <string>

using namespace std;

class Hash;
class Texture;

class StarBox {

public:
   StarBox();

   void setHash(Hash* h);
   void createStars();

private:
   void createBoxSide(Texture* texture, const Vector3D& translate, const Vector3D& rotate);

   Texture* yzNegX;
   Texture* yzPosX;
   Texture* xyNegZ;
   Texture* xyPosZ;
   Texture* xzNegY;
   Texture* xzPosY;
   int halfSize;
};

#endif

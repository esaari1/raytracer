#ifndef LensFlare_Murphy_h
#define LensFlare_Murphy_h

#include <SDL/SDL.h>
#include "Utility/SDL_Utility.h"
#include "Utility/Color.h"

class Murphy {

public:
   Murphy(SDL_Surface* s, const RGBAColor& c1, const RGBAColor& c2, bool sa);
   void wideline(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 width);

private:
   void murphyParaline(Sint16 x, Sint16 y, int d1);

   SDL_Surface* surf;
   RGBAColor color1, color2;
   bool swapAlpha;
   int u, v; /* delta x and y */
   int ku, kt, kv, kd;
   int oct2;
   int quad4;
   Sint16 tempx, tempy;
};

#endif

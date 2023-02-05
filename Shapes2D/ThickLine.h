#ifndef LensFlare_ThickLine_h
#define LensFlare_ThickLine_h

#include "Shape2D.h"
#include "Utility/Color.h"

class ThickLine : public Shape2D {

public:
   ThickLine(int x, int y, int length, float angle, int w, const RGBAColor& c1, const RGBAColor& c2);

   virtual void draw(SDL_Surface* surf);

private:
   int hw;
   int x1, y1, x2, y2;
   int length;
   float angle;
   bool swapAlpha;
   RGBAColor color1, color2;

   void drawHorizontal(SDL_Surface* surf);
   void drawVertical(SDL_Surface* surf);
};

#endif


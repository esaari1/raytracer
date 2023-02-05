#ifndef LensFlare_FilledEllipse_h
#define LensFlare_FilledEllipse_h

#include "Shape2D.h"
#include "Utility/Color.h"

class Ellipse : public Shape2D {
   
public:
   Ellipse(int x, int y, int w, int h, const RGBAColor& c1, const RGBAColor& c2);
   
   virtual void draw(SDL_Surface* surf);
   
private:
   int cx, cy;
   double rx, ry;
   RGBAColor color1, color2;
   
   void horizontal(SDL_Surface* surf, int x1, int x2, Uint16 y);
   float getF(int x, int y);
};

#endif

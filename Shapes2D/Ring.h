#ifndef LensFlare_Ring_h
#define LensFlare_Ring_h

#include "Shape2D.h"
#include "Utility/Color.h"

class Ring : public Shape2D {

public:
   Ring(int x, int y, int i, int o, const RGBAColor& c1, const RGBAColor& c2);

   virtual void draw(SDL_Surface* surf);

private:
   int cx, cy;
   int iradius, oradius;
   RGBAColor innerC, outerC;
   
   double getDistance(double x, double y);
};

#endif


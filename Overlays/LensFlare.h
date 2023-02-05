#ifndef raytracer_LensFlare_h
#define raytracer_LensFlare_h

#include "Overlay.h"
#include "Math/Point3D.h"
#include "Math/Matrix.h"

class LensFlare : public Overlay {

public:
   virtual void setHash(Hash* hash);
   virtual void render(SDL_Surface* dest, int cx, int cy, int width, int height);

private:
   int getFarX(int cx, int width, float percent);
   int getNearX(int cx, int width, float percent);
   int getFarY(int cy, int height, float percent);
   int getNearY(int cy, int height, float percent);
   void rainbow(SDL_Surface* surface, int x, int y, int r, int t);

   float sigma, alpha;
};

#endif

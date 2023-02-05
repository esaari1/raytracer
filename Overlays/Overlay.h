#ifndef raytracer_Overlay_h
#define raytracer_Overlay_h

#include <SDL/SDL.h>
#include "Math/Point3D.h"

class Hash;

class Overlay {

public:
   virtual void setHash(Hash* hash) = 0;
   virtual void render(SDL_Surface* dest, int cx, int cy, int width, int height) = 0;

   Point3D position;
};

#endif

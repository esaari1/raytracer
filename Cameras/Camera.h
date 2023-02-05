#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <SDL/SDL.h>
#include <string.h>
#include "Math/Point3D.h"
#include "Math/Vector3D.h"
#include "Samplers/Sampler.h"

class Tracer;
class Hash;
class Film;

void* renderThread(void* arg);

class Camera {

public:
   Camera(string fname, int w, int h);
   virtual ~Camera();

   void render();
   void renderBounds(const SamplerBounds& bounds);

   void setPosition(const Point3D& loc) { eye = loc; }
   void setRotation(double x, double y, double z);
   void setSurface(SDL_Surface* s) { surface = s; }
   void setThreadParameters(int tc, int w, int h);
   void translate(double x, double y, double z);

   Hash* samplerHash;

private:
   void setHash(Hash* hash);
   void computeUVW(Hash* h);

   Point3D eye;
   Vector3D u, v, w;
   Tracer* tracer;
   SDL_Surface* surface;
   Film* film;

   int width;
   int height;
   
   float lensRadius;
   float f, fDivV, viewPlaneDistance;

   int threadCount;
   int boxw;
   int boxh;
};

#endif

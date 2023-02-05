#ifndef raytracer_Film_h
#define raytracer_Film_h

#include "Utility/Color.h"
#include "Math/Point2D.h"
#include "Math/Point3D.h"
#include "Math/Matrix.h"

class SDL_Surface;
class Filter;

struct Pixel {
   Pixel();
   Color getColor();
   void reset();

   float r, g, b;
   float weightSum;
};

class Film {
   
public:
   Film(int w, int h);
   ~Film();

   void addSample(double sx, double sy, const Color& c);
   void generateImage(SDL_Surface* surf, const Point3D& eye, float viewPlaneDist, const Matrix& m);
   void reset();
   
private:
   int width, height;
   Filter* filter;
   Pixel* pixels;
};

#endif

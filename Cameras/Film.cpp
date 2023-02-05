#include "Film.h"
#include "Utility/SDL_Utility.h"
#include "Utility/atomics.h"
#include "Filters/MitchellFilter.h"
#include "Filters/BoxFilter.h"
#include "Overlays/OverlayManager.h"
#include "Math/Ray.h"
#include <SDL/SDL.h>
#include <math.h>

Pixel::Pixel() : r(0), g(0), b(0), weightSum(0) {}

Color Pixel::getColor() {
   if(weightSum <= 0) {
      return Color();
   }
   Color c(r / weightSum, g / weightSum, b / weightSum);
   c.normalize();
   return c;
}

void Pixel::reset() {
   r = g = b = 0;
   weightSum = 0;
}

Film::Film(int w, int h) : width(w), height(h) {
   filter = new BoxFilter(1.0, 1.0);
   pixels = new Pixel[width * height];
}

Film::~Film() {
   delete filter;
   delete[] pixels;
}

void Film::addSample(double sx, double sy, const Color& c) {
   int x0 = max(0, (int) ceil(sx - filter->xWidth));
   int x1 = min(width-1, (int) floor(sx + filter->xWidth));
   int y0 = max(0, (int) ceil(sy - filter->yWidth));
   int y1 = min(height-1, (int) floor(sy + filter->yWidth));

   for(int y = y0; y < y1; y++) {
      for(int x = x0; x < x1; x++) {
         double weight = filter->evaluate(x - sx, y - sy);

         AtomicAdd( , weight * c.red);
         AtomicAdd(&pixels[width * y + x].g, weight * c.green);
         AtomicAdd(&pixels[width * y + x].b, weight * c.blue);
         AtomicAdd(&pixels[width * y + x].weightSum, weight);
      }
   }
}

void Film::generateImage(SDL_Surface* surf, const Point3D& eye, float viewPlaneDist, const Matrix& m) {
   for(int w = 0; w < width; w++) {
      for(int h = 0; h < height; h++) {
         setPixel(surf, w, height - h - 1, pixels[width * h + w].getColor());
      }
   }

   for(OverlayIter it = OverlayManager::instance().begin(); it != OverlayManager::instance().end(); ++it) {
      Vector3D v = (*it)->position - eye;
      Vector3D d = m * v;

      double x = viewPlaneDist * d.x / -d.z;
      x = width * 0.5 + x;
      double y = viewPlaneDist * d.y / d.z;
      y = height * 0.5 + y;

      (*it)->render(surf, x, y, width, height);
   }

   SDL_UpdateRect(surf, 0, 0, 0, 0);
}

void Film::reset() {
   for(int w = 0; w < width; w++) {
      for(int h = 0; h < height; h++) {
         pixels[width * h + w].reset();
      }
   }
}

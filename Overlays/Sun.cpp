#include "Sun.h"
#include "Shapes2D/Ellipse.h"
#include "Shapes2D/ThickLine.h"
#include "Utility/FalloffFilter.h"
#include "Utility/SDL_Utility.h"
#include "Utility/Gaussian.h"
#include "Parser/Hash.h"

Sun::~Sun() {
   SDL_FreeSurface(surface);
}

void Sun::setHash(Hash* hash) {
   radius = hash->getInteger("radius");
   spikeLength = hash->getInteger("spikeLength");
   numSpikes = hash->getInteger("numSpikes");
   startAngle = hash->getDouble("startAngle");
   color.set(hash->getValue("color")->getArray());
   position.set(hash->getValue("position")->getArray());
   sigma = hash->getDouble("sigma", 3.0);
   alpha = hash->getDouble("alpha", 6.0);

   create();
}

void Sun::create() {
   int size = spikeLength * 2;
   SDL_Surface* surf = createSurface(1600, size);
   Uint32 black = SDL_MapRGBA(surf->format, 0, 0, 0, 0);
   SDL_FillRect(surf, NULL, black);

   int cx = 800, cy = spikeLength;

   RGBAColor blue(0.1, 0.3, 0.8, 0);

   float offset = 360.f / numSpikes;

   for(float a = startAngle; a < 360 + startAngle; a += offset) {
      ThickLine line(cx, cy, spikeLength, a, radius/2-10, RGBA_WHITE, color);
      line.draw(surf);
   }

   ThickLine blue1(cx, cy, 800, 0, radius/2, RGBA_WHITE, blue);
   blue1.draw(surf);
   ThickLine blue2(cx, cy, 800, 180, radius/2, RGBA_WHITE, blue);
   blue2.draw(surf);

   Ellipse center(cx, cy, radius, radius, RGBA_WHITE, color);
   center.setFilter(new SmoothStepFilter(0.2, 1.0));
   center.draw(surf);

   int fSize;
   double* filter = CreateGaussianFilter(sigma, alpha, fSize);
   surface = Convolute(surf, filter, fSize);

   delete[] filter;
   SDL_FreeSurface(surf);
}

void Sun::render(SDL_Surface* dest, int cx, int cy, int width, int height) {
   int l = (int)(cx - surface->w * 0.5);
   int t = (int)(cy - surface->h * 0.5);
   SDL_Rect dst = {l, t, 0, 0};
   SDL_BlitSurface(surface, NULL, dest, &dst);
}


SunHalo::~SunHalo() {
   SDL_FreeSurface(surface);
}

void SunHalo::setHash(Hash* hash) {
   radius = hash->getInteger("radius", 0);
   haloRadius = hash->getInteger("haloRadius");
   haloRadius2 = hash->getInteger("haloRadius2", haloRadius * 1/3);
   color1.set(hash->getValue("color1")->getArray());
   color2.set(hash->getValue("color2")->getArray());
   position.set(hash->getValue("position")->getArray());
   sigma = hash->getDouble("sigma", 3.0);
   alpha = hash->getDouble("alpha", 6.0);

   create();
}

void SunHalo::create() {
   int size = haloRadius * 2;
   SDL_Surface* surf = createSurface(size, size);
   Uint32 black = SDL_MapRGBA(surf->format, 0, 0, 0, 0);
   SDL_FillRect(surf, NULL, black);

   Ellipse haloA(haloRadius, haloRadius, haloRadius, haloRadius, RGBAColor(color2, 0.5), RGBAColor(color2, 0.0));
   haloA.draw(surf);

   Ellipse halo(haloRadius, haloRadius, haloRadius2, haloRadius2, RGBAColor(color1, 1.0), RGBAColor(color1, 0.0));
   halo.draw(surf);

   if(radius > 0) {
      Ellipse center(haloRadius, haloRadius, radius, radius, RGBA_WHITE, RGBAColor(1, 1, 1, 0));
      center.setFilter(new SmoothStepFilter(0.9, 1.0));
      center.draw(surf);
   }

   int fSize;
   double* filter = CreateGaussianFilter(sigma, alpha, fSize);
   surface = Convolute(surf, filter, fSize);

   delete[] filter;
   SDL_FreeSurface(surf);
}

void SunHalo::render(SDL_Surface* dest, int cx, int cy, int width, int height) {
   int l = (int)(cx - surface->w * 0.5);
   int t = (int)(cy - surface->h * 0.5);
   SDL_Rect dst = {l, t, 0, 0};
   SDL_BlitSurface(surface, NULL, dest, &dst);
}

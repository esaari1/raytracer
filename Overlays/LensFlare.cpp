#include "LensFlare.h"
#include "Parser/Hash.h"
#include "Utility/Color.h"
#include "Utility/FalloffFilter.h"
#include "Utility/SDL_Utility.h"
#include "Utility/Gaussian.h"
#include "Shapes2D/Ellipse.h"

float a = 0.15f;

void LensFlare::setHash(Hash* hash) {
   position.set(hash->getValue("position")->getArray());
   sigma = hash->getDouble("sigma", 3.0);
   alpha = hash->getDouble("alpha", 6.0);
}

void LensFlare::render(SDL_Surface* dest, int cx, int cy, int width, int height) {
   SDL_Surface* surface = createSurface(width, height);

   // big green
   RGBAColor color1(0, 0, 0, 0);
   RGBAColor color2(0.20, 0.45, 0, a);
   Ellipse circle1(width - cx, height - cy, 60, 60, color1, color2);
   circle1.draw(surface);

   // small green
   color1.set(0.20, 0.45, 0.2, a);
   color2.set(0.20, 0.45, 0.2, a);
   Ellipse circle2(getFarX(cx, width, 0.65), getFarY(cy, height, 0.65), 20, 20, color1, color2);
   circle2.draw(surface);

   // big orange
   color1.set(0, 0, 0, 0);
   color2.set(0.47, 0.27, 0, a);
   int x = getFarX(cx, width, 0.45);
   int y = getFarY(cy, height, 0.45);

   Ellipse circle3(x, y, 45, 45, color1, color2);
   circle3.draw(surface);

   // small orange
   color1.set(0.47, 0.27, 0, a);
   Ellipse circle4(x, y, 10, 10, color1, color2);
   circle4.draw(surface);

   // medium orange
   Ellipse circle5(getFarX(cx, width, 0.4), getFarY(cy, height, 0.4), 20, 20, color1, color2);
   circle5.draw(surface);

   // small orange 2
   Ellipse circle9(getNearX(cx, width, 0.1), getNearY(cy, height, 0.1), 20, 20, color1, color2);
   circle9.draw(surface);

   // big blue
   color1.set(0.13, 0.22, 0.36, a);
   color2.set(0.13, 0.22, 0.36, a);
   Ellipse circle6(getNearX(cx, width, 0.4), getNearY(cy, height, 0.4), 35, 35, color1, color2);
   circle6.draw(surface);

   // small blue 1
   Ellipse circle7(getNearX(cx, width, 0.37), getNearY(cy, height, 0.37), 15, 15, color1, color2);
   circle7.draw(surface);

   // small blue 2
   Ellipse circle8(getNearX(cx, width, 0.44), getNearY(cy, height, 0.44), 12, 12, color1, color2);
   circle8.draw(surface);

   // center dot
   color1.set(1, 1, 1, 1);
   color2.set(.6, .7, .9, 0);
   Ellipse circle10(width/2, height/2, 6, 6, color1, color2);
   circle10.draw(surface);

   // other dot
   Ellipse circle11(getFarX(cx, width, 0.3), getFarY(cy, height, 0.3), 7, 7, color1, color2);
   circle11.draw(surface);

   // white
   color1.set(0, 0, 0, 0);
   color2.set(0.59, 0.52, 0.42, a);
   Ellipse circle12(getNearX(cx, width, 1.5), getNearY(cy, height, 1.5), 35, 35, color1, color2);
   circle12.draw(surface);

   rainbow(surface, getFarX(cx, width, 1.4), getFarY(cy, height, 1.4), 200, 3);

   int fSize;
   double* filter = CreateGaussianFilter(sigma, alpha, fSize);
   SDL_Surface* convoluted = Convolute(surface, filter, fSize);

   delete[] filter;
   SDL_FreeSurface(surface);

   SDL_BlitSurface(convoluted, NULL, dest, NULL);
}

int LensFlare::getFarX(int cx, int width, float percent) {
   static int centerX = width / 2;

   int x = abs((width-cx) - centerX) * percent;
   if(cx < centerX) {
      x += centerX;
   }
   else {
      x = centerX - x;
   }
   return x;
}

int LensFlare::getNearX(int cx, int width, float percent) {
   static int centerX = width / 2;

   int x = abs((width-cx) - centerX) * percent;
   if(cx < centerX) {
      x = centerX - x;
   }
   else {
      x += centerX;
   }
   return x;
}

int LensFlare::getFarY(int cy, int height, float percent) {
   static int centerY = height / 2;

   int y = abs((height-cy) - centerY) * percent;
   if(cy < centerY) {
      y += centerY;
   }
   else {
      y = centerY - y;
   }
   return y;
}

int LensFlare::getNearY(int cy, int height, float percent) {
   static int centerY = height / 2;

   int y = abs((height-cy) - centerY) * percent;
   if(cy < centerY) {
      y = centerY - y;
   }
   else {
      y += centerY;
   }
   return y;
}

void LensFlare::rainbow(SDL_Surface* surface, int x, int y, int r, int t) {
   RGBAColor black(0, 0, 0, 0);
   RGBAColor color(0.35, 0, 0, a);
   Ellipse r1(x, y, r, r, black, color);
   r1.setFilter(new SmoothStepFilter(0.8, 1.0));
   r1.draw(surface);

   r -= t;
   color.set(0.35, 0.35, 0, a);
   Ellipse r2(x, y, r, r, black, color);
   r2.setFilter(new SmoothStepFilter(0.8, 1.0));
   r2.draw(surface);

   r -= t;
   color.set(0, 0.35, 0, a);
   Ellipse r3(x, y, r, r, black, color);
   r3.setFilter(new SmoothStepFilter(0.8, 1.0));
   r3.draw(surface);

   r -= t;
   color.set(0, 0, 0.35, a);
   Ellipse r4(x, y, r, r, black, color);
   r4.setFilter(new SmoothStepFilter(0.8, 1.0));
   r4.draw(surface);

   r -= t;
   color.set(0.0, 0.0, 0.0, a);
   Ellipse r5(x, y, r, r, black, color);
   r5.setFilter(new SmoothStepFilter(0.8, 1.0));
   r5.draw(surface);
}

#include "Gaussian.h"
#include "Utility/SDL_Utility.h"
#include <math.h>

double* CreateGaussianFilter(float piSigma, float piAlpha, int& lSize) {
   lSize = ((int)(piAlpha * piSigma) / 2)*2 + 1; //force odd-size filters

   double* filter = new double[lSize * lSize];

   for (int x = 0; x < lSize ; x++) {
      long FakeX = x - long(floor(lSize / 2.0));
      for (int y = 0 ; y < lSize ; y++) {
         long FakeY = y - long(floor(lSize / 2.0));
         double k = 1.0 / (2.0 * M_PI * piSigma * piSigma);
         filter[y * lSize + x] = k * exp( (-1 * ((FakeX * FakeX) + (FakeY *FakeY))) / (2 * piSigma * piSigma));
      }
   }
   //normalise the filter
   double lTotal=0;
   for (long x = 0; x < lSize; x++) {
      for (long y = 0;y < lSize; y++) {
         lTotal += filter[y * lSize + x];
      }
   }

   for (long x = 0;x < lSize; x++) {
      for (long y = 0;y < lSize; y++) {
         filter[y * lSize + x] /= lTotal;
      }
   }

   return filter;
}

SDL_Surface* Convolute(SDL_Surface* source, double* filter, int fSize) {
   SDL_Surface* dest = createSurface(source->w, source->h);
   Uint8 r, g, b, a;
   const int halfSize = (int)floor(fSize / 2.0);
   const double cf = 1.0 / 255.0;

   //for each pixel
   for (int x = 0; x < source->w; x++) {
      for (int y = 0; y < source->h; y++) {
         double SumR = 0;
         double SumG = 0;
         double SumB = 0;
         double SumA = 0;

         //For each point of the filter.
         for (int i = 0; i < fSize; i++) {
            //This is to make our origin in the center of the filter
            int FakeI = i - halfSize;

            if(y+FakeI < source->h && y+FakeI >= 0) {
               for (int j = 0; j < fSize; j++) {
                  int FakeJ = j - halfSize;

                  if (x+FakeJ < source->w && x+FakeJ >= 0) {
                     Uint32 pixel = get_pixel(source, x + FakeJ, y + FakeI);
                     SDL_GetRGBA(pixel, source->format, &r, &g, &b, &a);
                     SumR += r * filter[i * fSize + j] * cf;
                     SumG += g * filter[i * fSize + j] * cf;
                     SumB += b * filter[i * fSize + j] * cf;
                     SumA += a * filter[i * fSize + j] * cf;
                  }
               }
            }
         }

         Uint32 pixel = SDL_MapRGBA(source->format, SumR * 255, SumG * 255, SumB * 255, SumA * 255);
         setPixel(dest, x, y, pixel);
      }
   }

   return dest;
}

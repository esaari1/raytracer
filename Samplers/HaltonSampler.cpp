#include "HaltonSampler.h"
#include "Parser/Hash.h"
#include "Math/Maths.h"
#include <algorithm>

HaltonSampler::HaltonSampler(const SamplerBounds& bounds) : Sampler(bounds), current(0),delta(0) {
}

void HaltonSampler::setHash(Hash* h) {
   int spp = h->getInteger("samplesPerPixel");
   delta = max(xend - xstart, yend - ystart);
   nSamples = spp * delta * delta;
}

uint32_t HaltonSampler::getSamples(Sample* samples) {
   if (current >= nSamples) return 0;
   
   float imageX = xend;
   float imageY = yend;

   // Generate sample with Halton sequence and reject if outside image extent
   while(imageX >= xend || imageY >= yend) {
      float u = (float) radicalInverse(current, 3);
      float v = (float) radicalInverse(current, 2);

      imageX = lerp(u, (float) xstart, (float)(xstart + delta));
      imageY = lerp(v, (float) ystart, (float)(ystart + delta));
      ++current;
   }

   samples->imageX = imageX;
   samples->imageY = imageY;
   samples->lensX = (float) radicalInverse(current, 5);
   samples->lensY = (float) radicalInverse(current, 7);

   return 1;
}

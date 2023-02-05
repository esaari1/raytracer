#include "Regular.h"
#include <math.h>

Regular::Regular(const SamplerBounds& bounds) : Sampler(bounds), xPos(bounds.xstart), yPos(bounds.ystart) {
   nSamples = 1;
}

Regular::~Regular() {
}

void Regular::setHash(Hash* h) {
}

uint32_t Regular::getSamples(Sample* samples) {
   if(yPos == yend) return 0;

   samples->imageX = xPos + 0.5;
   samples->imageY = yPos + 0.5;
   samples->lensX = 0.5;
   samples->lensY = 0.5;

   if(++xPos == xend) {
      xPos = xstart;
      yPos++;
   }

   return nSamples;
}

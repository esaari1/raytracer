#include "StratifiedSampler.h"
#include "Parser/Hash.h"
#include "Math/Maths.h"
#include <stdlib.h>

StratifiedSampler::StratifiedSampler(const SamplerBounds& bounds) :
   Sampler(bounds), buffer(NULL), strataX(0), strataY(0), xPos(bounds.xstart), yPos(bounds.ystart)
{
}

StratifiedSampler::~StratifiedSampler() {
   delete[] buffer;
}

void StratifiedSampler::setHash(Hash* h) {
   strataX = h->getInteger("strataX");
   strataY = h->getInteger("strataY");

   nSamples = strataX * strataY;
   buffer = new float[nSamples * 4];
}

uint32_t StratifiedSampler::getSamples(Sample* samples) {
   if(yPos == yend) return 0;

   float* bufp = buffer;
   float* imageSamples = bufp; bufp += 2 * nSamples;
   float* lensSamples = bufp;  bufp += 2 * nSamples;

   sample2D(imageSamples, strataX, strataY, xPos, yPos);
   sample2D(lensSamples, strataX, strataY, 0, 0);

   Shuffle(lensSamples, nSamples, 2);

   for(uint32_t i = 0; i < nSamples; i++) {
      samples[i].imageX = imageSamples[2 * i];
      samples[i].imageY = imageSamples[2 * i + 1];
      samples[i].lensX = lensSamples[2 * i];
      samples[i].lensY = lensSamples[2 * i + 1];
   }

   if(++xPos == xend) {
      xPos = xstart;
      yPos++;
   }

   return nSamples;
}

void StratifiedSampler::sample1D(float* samples, int nSamples, int offset) {
   float invTot = 1.f / nSamples;
   for(int i = 0; i < nSamples; i++) {
      float delta = rand_float();
      *samples++ = (i + delta) * invTot + offset;
   }
}

void StratifiedSampler::sample2D(float* samples, int nx, int ny, int xoffset, int yoffset) {
   float dx = 1.f / nx, dy = 1.f / ny;
   for(int y = 0; y < ny; y++) {
      for(int x = 0; x < nx; x++) {
         float jx = rand_float();
         float jy = rand_float();
         *samples++ = (x + jx) * dx + xoffset;
         *samples++ = (y + jy) * dy + yoffset;
      }
   }
}

#ifndef raytracer_StratifiedSampler_h
#define raytracer_StratifiedSampler_h

#include "Sampler.h"

class StratifiedSampler : public Sampler {
   
public:
   StratifiedSampler(const SamplerBounds& bounds);
   virtual ~StratifiedSampler();

   virtual void setHash(Hash* h);
   virtual uint32_t getSamples(Sample* samples);
   
private:
   void sample1D(float* samples, int nSamples, int offset);
   void sample2D(float* samples, int nx, int n, int xoffset, int yoffset);

   float* buffer;
   int strataX, strataY;
   uint32_t xPos, yPos;
};

#endif

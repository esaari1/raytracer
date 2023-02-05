#ifndef raytracer_HaltonSampler_h
#define raytracer_HaltonSampler_h

#include "Sampler.h"

class HaltonSampler : public Sampler {
   
public:
   HaltonSampler(const SamplerBounds& bounds);
   
   virtual void setHash(Hash* h);
   virtual uint32_t getSamples(Sample* samples);
   
private:
   uint32_t current, delta;
};

#endif

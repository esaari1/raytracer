#ifndef _REGULAR_H_
#define _REGULAR_H_

#include "Sampler.h"

class Regular : public Sampler {

public:
   Regular(const SamplerBounds& bounds);
   virtual ~Regular();

   virtual void setHash(Hash* h);
   virtual uint32_t getSamples(Sample* samples);

private:
   uint32_t xPos, yPos;
};

#endif

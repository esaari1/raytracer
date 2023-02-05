#ifndef _MITCHElL_FILTER_H_
#define _MITCHElL_FILTER_H_

#include "Filter.h"

class MitchellFilter : public Filter {
   
public:
   MitchellFilter(float wx, float wy, float b = 0.f, float c = 0.5f);
   virtual float evaluate(float x, float y) const;
   
private:
   float mitchell1D(float x) const;
   float B, C, factor;
};

#endif

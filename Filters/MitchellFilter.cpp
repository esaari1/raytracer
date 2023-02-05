#include "MitchellFilter.h"
#include <math.h>

MitchellFilter::MitchellFilter(float wx, float wy, float b, float c) : Filter(wx, wy), B(b), C(c), factor(1.f / 6.f) {
}

float MitchellFilter::evaluate(float x, float y) const {
   return mitchell1D(x * invXWidth) * mitchell1D(y * invYWidth);
}

float MitchellFilter::mitchell1D(float x) const {
   x = fabs(x);
   if(x < 1.f) {
      return ((12 - 9*B - 6*C) * x*x*x + (-18 + 12*B + 6*C) * x*x + (6 - 2*B)) * factor;      
   }
   if(x < 2) {
      return ((-B - 6*C) * x*x*x + (6*B + 30*C) * x*x + (-12*B -48*C) * x + (8*B + 24*C)) * factor;
   }
   return 0;
}

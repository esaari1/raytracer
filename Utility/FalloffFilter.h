#ifndef FALLOFFFILTER_H
#define FALLOFFFILTER_H

#include "Math/Maths.h"
#include <math.h>

/** Main interface */
class FalloffFilter {

public:
   virtual double filter(double f) const = 0;
};

/** Linear filter */
class LinearFilter : public FalloffFilter {

public:
   virtual double filter(double f) const { return f; }
};

/** Cosine filter */
class CosineFilter : public FalloffFilter {

public:
   virtual double filter(double f) const {
      return 1.f - cos(f * M_PI / 2.f);
   }
};

/** Smoothstep filter */
class SmoothStepFilter : public FalloffFilter {

public:
   SmoothStepFilter(double e0, double e1) : edge0(e0), edge1(e1) {}

   virtual double filter(double f) const {
      return smootherstep(edge0, edge1, f);
   }

private:
   double edge0, edge1;
};

#endif // FALLOFFFILTER_H

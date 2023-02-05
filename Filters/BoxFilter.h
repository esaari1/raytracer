#ifndef _BOX_FILTER_H_
#define _BOX_FILTER_H_

#include "Filter.h"

class BoxFilter : public Filter {
   
public:
   BoxFilter(float xw, float yw) : Filter(xw, yw) {}
   virtual float evaluate(float x, float y) const { return 1.f; }
};

#endif

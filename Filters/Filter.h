#ifndef _FILTER_H_
#define _FILTER_H_

class Filter {
   
public:
   Filter(float xw, float yw) : xWidth(xw), yWidth(yw), invXWidth(1.f / xw), invYWidth(1.f / yw) {}
   virtual float evaluate(float x, float y) const = 0;
   
   const float xWidth, yWidth;
   const float invXWidth, invYWidth;
};

#endif

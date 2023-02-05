#ifndef raytracer_Transparency_h
#define raytracer_Transparency_h

#include "BRDF.h"

class Texture;

/**
 * Use this BRDF to return alpha values from an alpha texture or a constant alpha value.
 */
class Transparency : public BRDF {
  
public:
   Transparency();
   ~Transparency();
   
   virtual double getAlpha(const ShadeRecord& sr) const;

   void setAlpha(double a) { alpha = a; }
   
private:
   double alpha;
};

#endif

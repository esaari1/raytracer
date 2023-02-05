#ifndef _TRACER_H_
#define _TRACER_H_

#include "Utility/Color.h"
#include "Utility/ShadeRecord.h"
#include "Math/Ray.h"
#include "Textures/Texture.h"
#include "DirectIntegrator.h"

class Tracer {

public:
   Tracer();
   ~Tracer();

   Color traceRay(const Ray& ray, bool useBackground = true);

   void setBackgroundColor(const Color& c) { bgColor = c; }
   void setBackgroundTexture(Texture* t) { texture = t; }

protected:
   Color bgColor;
   Texture* texture;
   DirectIntegrator integrator;
};

#endif

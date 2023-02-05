#ifndef _DIRECT_INTEGRATOR_H_
#define _DIRECT_INTEGRATOR_H_

#include "Utility/Color.h"
#include "Utility/ShadeRecord.h"
#include "Math/Ray.h"

class Light;
class Tracer;

class DirectIntegrator {

public:
   DirectIntegrator();

   Color shade(ShadeRecord& sr, const Ray& ray, Tracer* tracer);

private:
   Color estimateDirect(ShadeRecord& sr, Light* light, const Vector3D& wo, float* samples, int s);
   Color specularReflect(ShadeRecord& sr, const Ray& ray, Tracer* tracer);
   Color specularTransmit(ShadeRecord& sr, const Ray &ray, Tracer* tracer);
   Color transparency(ShadeRecord& sr, const Ray &ray, Tracer* tracer, Color& L);
};

#endif

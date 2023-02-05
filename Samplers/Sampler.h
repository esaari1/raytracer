#ifndef raytracer_Sampler_h
#define raytracer_Sampler_h

#include "Math/Vector3D.h"
#include <stdint.h>

class Hash;

struct SamplerBounds {
   SamplerBounds(uint32_t xs = 0, uint32_t xe = 0, uint32_t ys = 0, uint32_t ye = 0);
   SamplerBounds& operator=(const SamplerBounds& other);
   uint32_t xstart, xend, ystart, yend;
};

struct Sample {
   float imageX, imageY;
   float lensX, lensY;
};

class Sampler {
   
public:
   Sampler(const SamplerBounds& bounds);
   virtual ~Sampler();
   
   static Sampler* createSampler(const SamplerBounds& bounds, Hash* h);
   
   void Shuffle(float* samp, uint32_t count, uint32_t dims);
   
   virtual void setHash(Hash* h) = 0;
   virtual uint32_t getSamples(Sample* samples) = 0;
   uint32_t getNumSamples() const { return nSamples; }
   
   static void LatinHyperCube(float* samples, uint32_t nSamples, uint32_t nDim);
   static void mapToDisk(float u, float v, float* du, float* dv);
   static Vector3D mapToHemisphere(float u, float v);
   static Vector3D uniformSampleCone(double u, double v, float costhetamax, const Vector3D& x, const Vector3D& y, const Vector3D& z);

protected:
   uint32_t xstart, xend, ystart, yend;
   uint32_t nSamples;
};

#endif

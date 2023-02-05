#include "Sampler.h"
#include "Regular.h"
#include "StratifiedSampler.h"
#include "HaltonSampler.h"
#include "Parser/Hash.h"
#include "Math/Maths.h"
#include <math.h>
#include <stdexcept>

SamplerBounds::SamplerBounds(uint32_t xs, uint32_t xe, uint32_t ys, uint32_t ye) :
   xstart(xs), 
   xend(xe), 
   ystart(ys), 
   yend(ye)
{   
}

SamplerBounds& SamplerBounds::operator=(const SamplerBounds& other) {
   xstart = other.xstart;
   xend = other.xend;
   ystart = other.ystart;
   yend = other.yend;
   return *this;
}

Sampler::Sampler(const SamplerBounds& bounds) : 
   xstart(bounds.xstart), 
   xend(bounds.xend), 
   ystart(bounds.ystart), 
   yend(bounds.yend), 
   nSamples(0)
{
}

Sampler::~Sampler() {
}

void Sampler::LatinHyperCube(float* samples, uint32_t nSamples, uint32_t nDim) {
   float delta = 1.f / nSamples;
   for(uint32_t i = 0; i < nSamples; i++) {
      for(uint32_t j = 0; j < nDim; j++) {
         samples[nDim * i + j] = (i + rand_float()) * delta;
      }
   }
   
   for(uint32_t i = 0; i < nDim; i++) {
      for (int j = 0; j < nSamples; j++) {
         uint32_t other = j + (rand() % (nSamples - j));
         swap(samples[nDim * j + i], samples[nDim * other + i]);
      }
   }
}

Sampler* Sampler::createSampler(const SamplerBounds& bounds, Hash* h) {
   string type = h->getString("type");
   Sampler* sampler;
   
   if(type == "regular") {
      sampler = new Regular(bounds);
   }
   else if(type == "stratified") {
      sampler = new StratifiedSampler(bounds);
   }
   else if(type == "halton") {
      sampler = new HaltonSampler(bounds);
   }
   else {
      throw runtime_error("Unknown sampler type " + type);
   }
   
   sampler->setHash(h);
   return sampler;
}

void Sampler::Shuffle(float* samp, uint32_t count, uint32_t dims) {
   for (uint32_t i = 0; i < count; ++i) {
      uint32_t other = i + (rand() % (count - i));
      for (uint32_t j = 0; j < dims; ++j)
         swap(samp[dims*i + j], samp[dims*other + j]);
   }
}

void Sampler::mapToDisk(float u, float v, float* dx, float* dy) {
   float r, theta;
   
   // Mao samples from [0, 1] to [-1, 1]
   float sx = 2 * u - 1;
   float sy = 2 * v - 1;
   
   // Handle degeneracy at the origin
   if (sx == 0.0 && sy == 0.0) {
      *dx = 0.0;
      *dy = 0.0;
      return;
   }
   if (sx >= -sy) {
      if (sx > sy) {
         // Handle first region of disk
         r = sx;
         theta = (sy > 0.0) ? sy / r : 8.f + sy / r;
      }
      else {
         // Handle second region of disk
         r = sy;
         theta = 2.0f - sx / r;
      }
   }
   else {
      if (sx <= sy) {
         // Handle third region of disk
         r = -sx;
         theta = 4.0f - sy / r;
      }
      else {
         // Handle fourth region of disk
         r = -sy;
         theta = 6.0f + sx / r;
      }
   }
   
   theta *= M_PI / 4.f;
   *dx = r * cosf(theta);
   *dy = r * sinf(theta);
}

Vector3D Sampler::mapToHemisphere(float u, float v) {
   float x, y;
   mapToDisk(u, v, &x, &y);
   float z = sqrtf(max(0.f, 1.f - x * x - y * y));
   return Vector3D(x, y, z);
}

Vector3D Sampler::uniformSampleCone(double u, double v, float costhetamax, const Vector3D& x, const Vector3D& y, const Vector3D& z) {
   double costheta = lerp<double>(u, costhetamax, 1.0);
   double sintheta = sqrt(1.0 - costheta * costheta);
   double phi = v * 2.0 * M_PI;
   return x * cos(phi) * sintheta + y * sinf(phi) * sintheta + z * costheta;
}

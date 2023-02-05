#include "DirectIntegrator.h"
#include "Materials/Material.h"
#include "Lights/LightManager.h"
#include "Lights/Light.h"
#include "BRDFs/Lambertian.h"
#include "Tracer.h"

DirectIntegrator::DirectIntegrator() {
}

Color DirectIntegrator::shade(ShadeRecord& sr, const Ray& ray, Tracer* tracer) {
   int maxDepth = 10;
   sr.material->applyNormalMap(sr);
   Vector3D wo = ray.direction * -1;
   Color L;

   for(LightIter it = LightManager::instance().begin(); it != LightManager::instance().end(); it++) {
      Color Ld;
      float* samples = (*it)->getSamples();
      for(int s = 0; s < (*it)->getNumLightSamples(); s++) {
         Ld += estimateDirect(sr, *it, wo, samples, s);
      }
      L += Ld / (*it)->getNumLightSamples();
   }

   if (ray.depth + 1 < maxDepth) {
      L = transparency(sr, ray, tracer, L);
      L += specularReflect(sr, ray, tracer);
      L += specularTransmit(sr, ray, tracer);
   }
   return L;
}

Color DirectIntegrator::estimateDirect(ShadeRecord& sr, Light* light, const Vector3D& wo, float* samples, int s) {
   Color Ld;
   Vector3D wi;
   float pdf;
   
   Color Li = light->Sample_L(sr, samples[s * 2], samples[s * 2 + 1], wi, pdf);
   if(pdf > 0 && !Li.isBlack()) {
      Color f = sr.material->bsdf.f(sr, wo, wi);
      if(!f.isBlack()) {
         float ndotwi = sr.normal.dot(wi);
         if(ndotwi > 0) {
            Ray shadowRay(sr.hitPoint, wi);
            bool inShadow = light->inShadow(shadowRay, sr);
            if(!inShadow) {
               Ld += f * Li * ndotwi/ pdf;
            } else {
               Ld += sr.material->ambientBRDF->rho(sr, wo) * LightManager::instance().getAmbientLight(sr);
            }
         }
      }
   }
   return sr.material->getLe(sr) + Ld;
}

Color DirectIntegrator::specularReflect(ShadeRecord& sr, const Ray& ray, Tracer* tracer) {
   Vector3D wo = -ray.direction;
   Vector3D wi;
   float pdf;
   const Point3D& p = sr.hitPoint;
   const Vector3D& n = sr.normal;
   Color f = sr.material->bsdf.sample_f(sr, wo, wi, pdf, BxDFType(REFLECT | SPECULAR));
   Color L = BLACK;

   float widotn = fabs(wi.dot(n));
   if (pdf > 0.f && !f.isBlack() && widotn != 0.f) {
      // Compute ray differential _rd_ for specular reflection
      Ray rd(p, wi, ray.depth + 1);
 
      Color Li = tracer->traceRay(rd, false);
      L = f * Li * widotn / pdf;
   }
   return L;
}

Color DirectIntegrator::specularTransmit(ShadeRecord& sr, const Ray &ray, Tracer* tracer) {
   Vector3D wo = -ray.direction;
   Vector3D wi;
   float pdf;
   const Point3D& p = sr.hitPoint;
   const Vector3D& n = sr.normal;
   Color f = sr.material->bsdf.sample_f(sr, wo, wi, pdf, BxDFType(TRANSMIT | SPECULAR));
   Color L = BLACK;
 
   float widotn = fabs(wi.dot(n));  
   if (pdf > 0.f && !f.isBlack() && widotn != 0.f) {
      Ray rd(p, wi, ray.depth + 1);
      
      Color Li = tracer->traceRay(rd);
      L = f * Li * widotn / pdf;
   }

   return L;
}

Color DirectIntegrator::transparency(ShadeRecord& sr, const Ray &ray, Tracer* tracer, Color& L) {
   double alpha = sr.material->bsdf.sampleAlpha(sr);
   if(alpha < 1.0) {
      Ray rd(sr.hitPoint, ray.direction, ray.depth + 1);
      Color Li = tracer->traceRay(rd);      
      return L * alpha + Li * (1.0 - alpha);
   }

   return L;
}

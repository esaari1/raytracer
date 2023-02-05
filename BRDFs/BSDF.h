#ifndef _BSDF_H_
#define _BSDF_H_

#include "BRDF.h"
#include <vector>

using namespace std;

class BSDF {

public:
   virtual ~BSDF();
   
   void addBRDF(BRDF* brdf);

   Color f(const ShadeRecord& sr, const Vector3D& wo, const Vector3D& wi) const;
   Color sample_f(const ShadeRecord& sr, const Vector3D& wo, Vector3D& wi, float& pdf, BxDFType flags) const;
   Color rho(const ShadeRecord& sr, const Vector3D& wo) const;
   double sampleAlpha(const ShadeRecord& sr) const;

   vector<BRDF*> getMatches(BxDFType flags) const;

private:
   vector<BRDF*> brdfs;
};

#endif

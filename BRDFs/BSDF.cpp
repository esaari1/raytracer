#include "BSDF.h"
#include "Math/Maths.h"

typedef vector<BRDF*>::const_iterator BRDFIter;

BSDF::~BSDF() {
   for(BRDFIter it = brdfs.begin(); it != brdfs.end(); ++it) {
      delete *it;
   }
   brdfs.clear();
}

void BSDF::addBRDF(BRDF* brdf) {
   brdfs.push_back(brdf);
}

Color BSDF::f(const ShadeRecord& sr, const Vector3D& wo, const Vector3D& wi) const {
   Color f;
   for(BRDFIter it = brdfs.begin(); it != brdfs.end(); ++it) {
      f += (*it)->f(sr, wo, wi);
   }
   return f;
}

Color BSDF::sample_f(const ShadeRecord& sr, const Vector3D& wo, Vector3D& wi, float& pdf, BxDFType flags) const {
   vector<BRDF*> matches = getMatches(flags);
   if(matches.size() == 0) { 
      return BLACK;
   }
   
   int which = matches.size() > 1 ? rand_int(0, (int)matches.size()-1) : 0;
   BRDF* brdf = matches[which];
   
   Color f = brdf->sample_f(sr, wo, wi, pdf);
   if(pdf == 0) { 
      return BLACK;
   }
   
   return f;
}

Color BSDF::rho(const ShadeRecord& sr, const Vector3D& wo) const {
   Color rho;
   for(BRDFIter it = brdfs.begin(); it != brdfs.end(); ++it) {
      rho += (*it)->rho(sr, wo);
   }
   return rho;
}

vector<BRDF*> BSDF::getMatches(BxDFType flags) const {
   vector<BRDF*> matches;
   for(BRDFIter it = brdfs.begin(); it != brdfs.end(); ++it) {
      if((*it)->matches(flags)) {
         matches.push_back(*it);
      }
   }
   return matches;
}

double BSDF::sampleAlpha(const ShadeRecord& sr) const {
   vector<BRDF*> matches = getMatches(BxDFType(TRANSPARENT));
   if(matches.size() == 0) { 
      return 1.0;
   }
   
   int which = matches.size() > 1 ? rand_int(0, (int)matches.size()-1) : 0;
   BRDF* brdf = matches[which];   
   return brdf->getAlpha(sr);
}

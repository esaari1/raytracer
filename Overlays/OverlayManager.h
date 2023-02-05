#ifndef raytracer_OverlayManager_h
#define raytracer_OverlayManager_h

#include <vector>
#include <memory>
#include <string>
#include <SDL/SDL.h>
#include "Overlay.h"

using namespace std;

typedef vector<Overlay*>::const_iterator OverlayIter;

class OverlayManager {
  
public:
   static OverlayManager& instance();
   ~OverlayManager();
   
   void loadOverlays(string fname);
   OverlayIter begin() const { return overlays.begin(); }
   OverlayIter end() const { return overlays.end(); }
   
private:
   OverlayManager();
   
   static auto_ptr<OverlayManager> s_instance;
   vector<Overlay*> overlays;
};

#endif

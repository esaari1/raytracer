#include "Ring.h"
#include "Utility/SDL_Utility.h"
#include "Math/Maths.h"
#include <math.h>

Ring::Ring(int x, int y, int i, int o, const RGBAColor& c1, const RGBAColor& c2) :
   cx(x), cy(y), iradius(i), oradius(o), innerC(c1), outerC(c2)
{
}

void Ring::draw(SDL_Surface* surf) {
   const int BPP = surf->format->BytesPerPixel;

   double r = (double)oradius;
   double ir = (double)iradius;
   double dr = (r - ir) * 0.5;
   double cr = (r + ir) * 0.5;

   for (double dy = 1; dy <= r; dy += 1.0) {
      double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
      int x = cx-dx;

      // Grab a pointer to the left-most pixel for each half of the circle
      Uint8 *target_pixel_a = (Uint8 *)surf->pixels + ((int)(cy + r - dy)) * surf->pitch + x * BPP;
      Uint8 *target_pixel_b = (Uint8 *)surf->pixels + ((int)(cy - r + dy)) * surf->pitch + x * BPP;
      
      int y = cy + r - dy;

      for (; x <= cx + dx; x++) {
         double dist = getDistance(x, y);
         if(dist > ir) {
            double f = (dist < cr) ? (dist - ir) / dr : 1.0 - (dist - cr) / dr;
            RGBAColor c = lerp(f, outerC, innerC);
            setBlendColor(surf, (Uint32*) target_pixel_a, c);
            setBlendColor(surf, (Uint32*) target_pixel_b, c);
         }
         target_pixel_a += BPP;
         target_pixel_b += BPP;
      }
   }
}

double Ring::getDistance(double x, double y) {
   double dx = x - cx;
   double dy = y - cy;
   return sqrt(dx*dx + dy*dy);
}


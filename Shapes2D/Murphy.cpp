#include "Murphy.h"
#include "Line.h"
#include "Math/Maths.h"
#include <algorithm>
#include <math.h>

using namespace std;

#define HYPOT(x,y) sqrt((double)(x)*(double)(x)+(double)(y)*(double)(y))

Murphy::Murphy(SDL_Surface* s, const RGBAColor& c1, const RGBAColor& c2, bool sa) :
   surf(s), color1(c1), color2(c2), swapAlpha(sa)
{
}

void Murphy::wideline(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 width) {
   float offset = (float)width / 2.f;

   Sint16 ptx, pty, ml1x, ml1y, ml2x, ml2y, ml1bx, ml1by, ml2bx, ml2by;

   /* Initialisation */
   u = x2 - x1;   /* delta x */
   v = y2 - y1;   /* delta y */

   if (u < 0) {   /* swap to make sure we are in quadrants 1 or 4 */
      swap(x1, x2);
      swap(y1, y2);
      u *= -1;
      v *= -1;
   }

   if (v < 0) {   /* swap to 1st quadrant and flag */
      v *= -1;
      quad4 = 1;
   } else {
      quad4 = 0;
   }

   if (v > u) {   /* swap things if in 2 octant */
      swap(u, v);
      oct2 = 1;
   } else {
      oct2 = 0;
   }

   ku = u + u; /* change in l for square shift */
   kv = v + v; /* change in d for square shift */
   kd = kv - ku;  /* change in d for diagonal shift */
   kt = u - kv;   /* diag/square decision threshold */

   int d0 = 0;
   int d1 = 0;
   int dd = 0;

   double ang = atan((double) v / (double) u);  /* calc new initial point - offset both sides of ideal */
   double sang = sin(ang);
   double cang = cos(ang);

   if (oct2 == 0) {
      ptx = x1 + (Sint16)lrint(offset * sang);
      if (quad4 == 0) {
         pty = y1 - (Sint16)lrint(offset * cang);
      } else {
         pty = y1 + (Sint16)lrint(offset * cang);
      }
   } else {
      ptx = x1 - (Sint16)lrint(offset * cang);
      if (quad4 == 0) {
         pty = y1 + (Sint16)lrint(offset * sang);
      } else {
         pty = y1 - (Sint16)lrint(offset * sang);
      }
   }

   /* used here for constant thickness line */
   int tk = (int) (4. * HYPOT(ptx - x1, pty - y1) * HYPOT(u, v));

   double maxDistance = sqrt(pow(ptx-x1, 2) + pow(pty-y1, 2));

   float a1 = color1.alpha;
   float a2 = color2.alpha;

   for (int q = 0; dd <= tk; q++) { /* outer loop, stepping perpendicular to line */
      double dist = sqrt(pow(ptx-x1, 2) + pow(pty-y1, 2));
      double f = (maxDistance - dist) / maxDistance;
      color1.alpha = a1 * f;
      color2.alpha = a2 * f;

      murphyParaline(ptx, pty, d1); /* call to inner loop - right edge */
      if (q == 0) {
         ml1x = ptx;
         ml1y = pty;
         ml1bx = tempx;
         ml1by = tempy;
      } else {
         ml2x = ptx;
         ml2y = pty;
         ml2bx = tempx;
         ml2by = tempy;
      }
      if (d0 < kt) { /* square move */
         if (oct2 == 0) {
            if (quad4 == 0) {
               pty++;
            } else {
               pty--;
            }
         } else {
            ptx++;
         }
      } else { /* diagonal move */
         dd += kv;
         d0 -= ku;
         if (d1 < kt) { /* normal diagonal */
            if (oct2 == 0) {
               ptx--;
               if (quad4 == 0) {
                  pty++;
               } else {
                  pty--;
               }
            } else {
               ptx++;
               if (quad4 == 0) {
                  pty--;
               } else {
                  pty++;
               }
            }
            d1 += kv;
         } else { /* double square move, extra parallel line */
            if (oct2 == 0) {
               ptx--;
            } else {
               if (quad4 == 0) {
                  pty--;
               } else {
                  pty++;
               }
            }
            d1 += kd;
            if (dd > tk) {
               return;  /* breakout on the extra line */
            }

            dist = sqrt(pow(ptx-x1, 2) + pow(pty-y1, 2));
            f = (maxDistance - dist) / maxDistance;
            color1.alpha = a1 * f;
            color2.alpha = a2 * f;

            murphyParaline(ptx, pty, d1);
            if (oct2 == 0) {
               if (quad4 == 0) {
                  pty++;
               } else {
                  pty--;
               }
            } else {
               ptx++;
            }
         }
      }
      dd += ku;
      d0 += kv;
   }
}

void Murphy::murphyParaline(Sint16 x, Sint16 y, int d1) {
   d1 = -d1;
   double length = sqrt(u*u + v*v);

   int ox = x;
   int oy = y;

   for (int p = 0; p <= u; p++) {
      double dist = sqrt(pow(x - ox, 2) + pow(y - oy, 2));
      double f = dist / length;
      if(swapAlpha) {
         f = 1.0 - f;
      }

      RGBAColor color = lerp(f, color1, color2);

      setBlendColor(surf, x, y, color);

      if (d1 <= kt) {
         if (oct2 == 0) {
            x++;
         } else {
            if (quad4 == 0) {
               y++;
            } else {
               y--;
            }
         }
         d1 += kv;
      } else {
         x++;
         if (quad4 == 0) {
            y++;
         } else {
            y--;
         }
         d1 += kd;
      }
   }

   tempx = x;
   tempy = y;
}

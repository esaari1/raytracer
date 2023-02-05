#include "Ellipse.h"
#include "Utility/SDL_Utility.h"
#include "Math/Maths.h"
#include "Utility/FalloffFilter.h"
#include <math.h>
#include <algorithm>

using namespace std;

Ellipse::Ellipse(int x, int y, int w, int h, const RGBAColor& c1, const RGBAColor& c2) :
   cx(x), cy(y), rx(w), ry(h), color1(c1), color2(c2)
{
}

void Ellipse::horizontal(SDL_Surface* surf, int x1, int x2, Uint16 y) {
   x1 = max(x1, (int)surf->clip_rect.x);
   x2 = min(x2, (int)(surf->clip_rect.x + surf->clip_rect.w - 1));

   for (int x = x1; x <= x2; x++) {
      float f = getF(x, y);
      RGBAColor c = lerp(f, color1, color2);
      setBlendColor(surf, x, y, c);
   }
}

void Ellipse::draw(SDL_Surface* surf) {
	int h, i, j, k;
	int xmh, xph;
	int xmi, xpi;
	int xmj, xpj;
	int xmk, xpk;

   // Get circle and clipping boundary and test if bounding box of circle is visible
   int oh = 0xFFFF;
   int oi = 0xFFFF;
   int oj = 0xFFFF;
   int ok = 0xFFFF;

   if (rx > ry) {
      int ix = 0;
      int iy = rx * 64;

      do {
         h = (ix + 32) >> 6;
         i = (iy + 32) >> 6;
         j = (h * ry) / rx;
         k = (i * ry) / rx;

         if ((ok != k) && (oj != k)) {
            xph = cx + h;
            xmh = cx - h;
            if (k > 0) {
               horizontal(surf, xmh, xph, cy + k);
               horizontal(surf, xmh, xph, cy - k);
            } else {
               horizontal(surf, xmh, xph, cy);
            }
            ok = k;
         }
         if ((oj != j) && (ok != j) && (k != j)) {
            xmi = cx - i;
            xpi = cx + i;
            if (j > 0) {
               horizontal(surf, xmi, xpi, cy + j);
               horizontal(surf, xmi, xpi, cy - j);
            } else {
               horizontal(surf, xmi, xpi, cy);
            }
            oj = j;
         }

         ix = ix + iy / rx;
         iy = iy - ix / rx;
      } while (i > h);
   } else {
      int ix = 0;
      int iy = ry * 64;

      do {
         h = (ix + 32) >> 6;
         i = (iy + 32) >> 6;
         j = (h * rx) / ry;
         k = (i * rx) / ry;

         if ((oi != i) && (oh != i)) {
            xmj = cx - j;
            xpj = cx + j;
            if (i > 0) {
               horizontal(surf, xmj, xpj, cy + i);
               horizontal(surf, xmj, xpj, cy - i);
            } else {
               horizontal(surf, xmj, xpj, cy);
            }
            oi = i;
         }
         if ((oh != h) && (oi != h) && (i != h)) {
            xmk = cx - k;
            xpk = cx + k;
            if (h > 0) {
               horizontal(surf, xmk, xpk, cy + h);
               horizontal(surf, xmk, xpk, cy - h);
            } else {
               horizontal(surf, xmk, xpk, cy);
            }
            oh = h;
         }

         ix = ix + iy / ry;
         iy = iy - ix / ry;
      } while (i > h);
   }
}

float Ellipse::getF(int x, int y) {
   x = x - cx;
   y = y - cy;

   if(x == 0 && y == 0) {
      return 0;
   }

   float px = rx * ry * x / sqrtf(powf(ry * x, 2) + powf(rx * y, 2));
   float py = rx * ry * y / sqrtf(powf(ry * x, 2) + powf(rx * y, 2));

   float dist = sqrt(x*x + y*y) / sqrt(px*px + py*py);
   return filter->filter(dist);
}

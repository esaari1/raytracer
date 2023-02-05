#include "Line.h"
#include "Utility/SDL_Utility.h"
#include "Math/Maths.h"
#include "Utility/FalloffFilter.h"
#include <algorithm>
#include <math.h>

Line::Line(int _x1, int _y1, int _x2, int _y2, const RGBAColor& c1, const RGBAColor& c2) :
   x1(_x1), y1(_y1), x2(_x2), y2(_y2), color1(c1), color2(c2)
{
   swapAlpha = false;
}

Line::Line(int x, int y, int length, float angle, const RGBAColor& c1, const RGBAColor& c2) {
   x1 = x;
   y1 = y;

   if(angle > 360) {
      angle -= 360;
   }
   if(angle < 0) {
      angle += 360;
   }

   swapAlpha = (angle > 180);
   angle *= M_PI / 180.f;
   x2 = (int)(length * cos(angle)) + x1;
   y2 = (int)(-length * sin(angle)) + y1;

   if(x1 == x2 || y1 == y2) {
      swapAlpha = false;
   }

   color1 = c1;
   color2 = c2;
}

void Line::draw(SDL_Surface* surf) {
   int ox = x1;
   int oy = y1;

   int32_t dx = x2 - x1;
   int32_t dy = y2 - y1;

   // special cases
   if(y1 == y2) {
      horizontal(surf, y1);
      return ;
   }
   if(x1 == x2) {
      vertical(surf, x1);
      return ;
   }

   if(y1 > y2) {
      std::swap(x1, x2);
      std::swap(y1, y2);
   }

   // Adjust for negative dx and set xdir
   int xdir = (dx >= 0) ? 1 : -1;
   dx = abs(dx);

   Uint32 erracc = 0;

   double length = sqrt(dx*dx + dy*dy);

   if(dy > dx) {
      Uint32 erradj = ((dx << 16) / dy) << 16;

      int x0pxdir = x1 + xdir;
      while(--dy) {
         int erracctmp = erracc;
         erracc += erradj;
         if(erracc <= erracctmp) {
            x1 = x0pxdir;
            x0pxdir += xdir;
         }

         y1++;

         double dist = sqrt(pow(x1-ox, 2) + pow(y1-oy, 2));
         double f = dist / length;
         if(!swapAlpha) {
            f = 1.0 - f;
         }
         f = filter->filter(f);
         RGBAColor c = lerp(f, color1, color2);
         Uint32 wgt = (erracc >> 24) & 255;
         setBlendColor(surf, x1, y1, c, 255 - wgt);
         setBlendColor(surf, x0pxdir, y1, c, wgt);
      }
   }
   else {
      Uint32 erradj = ((dy << 16) / dx) << 16;

      int y0p1 = y1 + 1;
      while(--dx) {
         int erracctmp = erracc;
         erracc += erradj;
         if(erracc <= erracctmp) {
            y1 = y0p1;
            y0p1++;
         }
         x1 += xdir;

         double dist = sqrt(pow(x1-ox, 2) + pow(y1-oy, 2));
         double f = dist / length;
         if(!swapAlpha) {
            f = 1.0 - f;
         }
         f = filter->filter(f);
         RGBAColor c = lerp(f, color1, color2);
         Uint32 wgt = (erracc >> 24) & 255;
         setBlendColor(surf, x1, y1, c, 255 - wgt);
         setBlendColor(surf, x1, y0p1, c, wgt);
      }
   }
}

void Line::horizontal(SDL_Surface* surf, int y) {
   if (x1 > x2) {
      std::swap(x1, x2);
      swapAlpha = true;
   }

   float dx = x2 - x1;

   for (int x = x1; x <= x2; x++) {
      float f = (x - x1) / dx;
      if(swapAlpha) f = 1.0 - f;
      f = filter->filter(f);
      RGBAColor c = lerp(f, color1, color2);
      setBlendColor(surf, x, y, c);
   }
}

void Line::vertical(SDL_Surface * surf, int x) {
   if (y1 > y2) {
      std::swap(y1, y2);
      swapAlpha = true;
   }

   float dy = y2 - y1;

   for (int y = y1; y <= y2; y++) {
      float f = (y - y1) / dy;
      if(swapAlpha) f = 1.0 - f;
      f = filter->filter(f);
      RGBAColor c = lerp(f, color1, color2);
      setBlendColor(surf, x, y, c);
   }
}


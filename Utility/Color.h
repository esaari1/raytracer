#ifndef _COLOR_H_
#define _COLOR_H_

#include <SDL/SDL.h>
#include "Parser/Value.h"

class Color {

public:
   Color(float _r = 0, float _g = 0, float _b = 0);
   Color(const Color& c);
   Color(Array* a);
   Color(const float rgb[3]);

   void set(float r, float b, float g);
   void set(Array* a);

   Color& operator=(const Color& c);
   Color& operator+=(const Color& c);
   Color& operator+=(const float a);
   Color& operator/= (const float a);
   Color& operator*= (const float a);
   bool operator== (const Color& c) const;
   Color operator*(const float a) const;
   Color operator*(const Color& c) const;
   Color operator/(const float a) const;
   Color operator+(const Color& c) const;
   Color operator-(const Color& c) const;
   Color operator-(const float a) const;

   bool isBlack() const;

   Array* toArray() const;

   void normalize();

   Uint8 getRed() const { return (Uint8)(red * 255); }
   Uint8 getGreen() const { return (Uint8)(green * 255); }
   Uint8 getBlue() const { return (Uint8)(blue * 255); }

   float red, green, blue;
};

class RGBAColor {

public:
   RGBAColor(float _r = 0, float _g = 0, float _b = 0, float _a = 0);
   RGBAColor(const RGBAColor& c);
   RGBAColor(const Color& c, float a = 1);

   void set(float r, float g, float b, float a);
   void set(Array* a);

   RGBAColor& operator=(const RGBAColor& c);
   RGBAColor operator+(const RGBAColor& c) const;
   RGBAColor operator-(const RGBAColor& c) const;
   RGBAColor operator*(const float a) const;

   Uint8 getAlpha() const { return (Uint8)(alpha * 255); }

   float red, green, blue, alpha;
};

Color operator*(const float a, const Color& c);

inline Color Color::operator*(const float a) const {
   return Color(red * a, green * a, blue * a);
}

inline Color Color::operator*(const Color& c) const {
   return Color(red * c.red, green * c.green, blue * c.blue);
}

inline Color Color::operator/(const float a) const {
   return Color(red / a, green / a, blue / a);
}

inline Color Color::operator+(const Color& c) const {
   return Color(red + c.red, green + c.green, blue + c.blue);
}

inline Color Color::operator-(const Color& c) const {
   return Color(red - c.red, green - c.green, blue - c.blue);
}

inline Color Color::operator-(const float a) const {
   return Color(red - a, green - a, blue - a);
}


// RGBAColor
inline RGBAColor RGBAColor::operator+(const RGBAColor& c) const {
   return RGBAColor(red + c.red, green + c.green, blue + c.blue, alpha + c.alpha);
}

inline RGBAColor RGBAColor::operator-(const RGBAColor& c) const {
   return RGBAColor(red - c.red, green - c.green, blue - c.blue, alpha - c.alpha);
}

inline RGBAColor RGBAColor::operator*(const float a) const {
   return RGBAColor(red * a, green * a, blue * a, alpha * a);
}

extern const Color BLACK;
extern const Color WHITE;
extern const RGBAColor RGBA_WHITE;

#endif

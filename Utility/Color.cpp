#include "Color.h"
#include <algorithm>
#include <math.h>

using namespace std;

const Color BLACK;
const Color WHITE(1, 1, 1);
const RGBAColor RGBA_WHITE(1, 1, 1, 1);

float epsilon = 1.0 * pow(10, -6);

Color::Color(float _r, float _g, float _b) {
   red = _r;
   green = _g;
   blue = _b;
}

Color::Color(const Color& c) {
   red = c.red;
   green = c.green;
   blue = c.blue;
}

Color::Color(Array* a) {
   red = a->at(0)->getDouble();
   green = a->at(1)->getDouble();
   blue = a->at(2)->getDouble();
}

Color::Color(const float rgb[3]) {
   red = rgb[0];
   green = rgb[1];
   blue = rgb[2];
}

void Color::set(float r, float g, float b) {
   red = r;
   green = g;
   blue = b;
}

void Color::set(Array* a) {
   red = a->at(0)->getDouble();
   green = a->at(1)->getDouble();
   blue = a->at(2)->getDouble();
}

Color& Color::operator= (const Color& p) {
   red = p.red;
   green = p.green;
   blue = p.blue;
   return *this;
}

Color& Color::operator+= (const Color& c) {
   red += c.red;
   green += c.green;
   blue += c.blue;
   return *this;
}

Color& Color::operator+=(const float a) {
   red += a;
   green += a;
   blue += a;
   return *this;
}

Color& Color::operator/= (const float a) {
   red /= a;
   green /= a;
   blue /= a;
   return *this;
}

Color& Color::operator*= (const float a) {
   red *= a;
   green *= a;
   blue *= a;
   return *this;
}

bool Color::operator== (const Color& c) const {
   if(fabs(red - c.red) > epsilon) {
      return false;
   }
   if(fabs(green - c.green) > epsilon) {
      return false;
   }
   if(fabs(blue - c.blue) > epsilon) {
      return false;
   }
   return true;
}

Array* Color::toArray() const {
   Array* a = new Array();
   a->push_back(new Value(red));
   a->push_back(new Value(green));
   a->push_back(new Value(blue));
   return a;
}

void Color::normalize() {
//   red = min(red, 1.0f);
//   green = min(green, 1.0f);
//   blue = min(blue, 1.0f);
//   static float g = 1.f / 1.8f;

   float f = max(red, max(green, blue));
   if(f > 1.f) {
      red /= f;
      green /= f;
      blue /= f;
   }

//   red = pow(red, g);
//   green = pow(green, g);
//   blue = pow(blue, g);
}

bool Color::isBlack() const {
   return *this == BLACK;
}

Color operator*(const float a, const Color& c) {
   return Color(a * c.red, a * c.green, a * c.blue);
}

RGBAColor::RGBAColor(float _r, float _g, float _b, float _a) {
   red = _r;
   green = _g;
   blue = _b;
   alpha = _a;
}

RGBAColor::RGBAColor(const RGBAColor& c) {
   red = c.red;
   green = c.green;
   blue = c.blue;
   alpha = c.alpha;
}

RGBAColor::RGBAColor(const Color& c, float a) {
   red = c.red;
   green = c.green;
   blue = c.blue;
   alpha = a;
}

RGBAColor& RGBAColor::operator= (const RGBAColor& p) {
   red = p.red;
   green = p.green;
   blue = p.blue;
   alpha = p.alpha;
   return *this;
}

void RGBAColor::set(float r, float g, float b, float a) {
   red = r;
   green = g;
   blue = b;
   alpha = a;
}

void RGBAColor::set(Array* a) {
   red = a->at(0)->getDouble();
   green = a->at(1)->getDouble();
   blue = a->at(2)->getDouble();
   alpha = a->at(3)->getDouble();
}

#ifndef _SPOTLIGHT_H_
#define _SPOTLIGHT_H_

#include "Light.h"

/**
 * Hash values:
 * <ul>
 * <li>location [ x y z ] - Location of the light source
 * <li>direction [ x y z ] - Direction of the center of the spotlight cone
 * <li>color [ r g b ] - Color of the light
 * <li>radiance <i>double</i> - Light intensity
 * <li>width <i>double</i> - Angular width of light cone
 * <li>falloff <i>double</i> - Angular width of start of light falloff
 * </ul>
 */
class SpotLight : public Light {

public:
   SpotLight();
   virtual ~SpotLight();
   
   virtual bool inShadow(const Ray& ray, const ShadeRecord& sr);
   virtual void setHash(Hash* hash);
   virtual Color Sample_L(ShadeRecord& sr, float u1, float u2, Vector3D& lightDir, float & pdf) const;
   virtual bool isDelta() const { return true; }
   virtual float* getSamples();

private:
   double falloff(const Point3D& p) const;

   float ls;
   Color color;
   double cosWidth;
   double cosFalloff;

   Point3D location;
   Vector3D direction;
   float* samples;
};

#endif

#ifndef raytracer_MarbleChecker_h
#define raytracer_MarbleChecker_h

#include "MarbleTexture.h"

/**
 * Hash values:
 * <ul>
 * <li>color1 [ r g b ] - Primary square color
 * <li>color2 [ r g b ] - Seconday square color
 * <li>size <i>value</i> - Size of each square in world units
 * </ul>
 */
class MarbleChecker : public Texture {
   
public:
   MarbleChecker();
   virtual ~MarbleChecker();
   
   virtual Color getColor(const ShadeRecord& sr) const;
   virtual void setHash(Hash* hash);
   
private:
   Hash* createHash(const Color& c, const Color& v, int seed);
   MarbleTexture* squares;
   float size;
};


#endif

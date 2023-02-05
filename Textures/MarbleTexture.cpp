#include "MarbleTexture.h"
#include "Noise/CubicNoise.h"
#include "Math/Maths.h"
#include "Parser/Hash.h"
#include <SDL/SDL.h>
#include <SDL_image.h>
#include "Utility/SDL_Utility.h"

MarbleTexture::MarbleTexture() : Texture(), mainColor(1.0, 1.0, 1.0), veinColor(0.1, 0.1, 0.1), noise(NULL) {
}

MarbleTexture::~MarbleTexture() {
   delete noise;
   SDL_FreeSurface(surf);
}

Color MarbleTexture::getColor(const ShadeRecord& sr) const {
   float value = a * noise->fbm(sr.localHitPoint);
   float y = sr.localHitPoint.y + value;
   float t = (1.0 + sin(y)) * 0.5;
   t = t * (surf->w - 1);
   Uint32 pixel = get_pixel(surf, t, 1);
   Uint8 r, g, b;
   SDL_GetRGB(pixel, surf->format, &r, &g, &b);
   
   return Color(r/255.f, g/255.f, g/255.f);
}

void MarbleTexture::setHash(Hash* hash) {
   noise = new CubicNoise();
   noise->setHash(hash);
   
   string image = hash->getString("image");
   surf = IMG_Load(image.c_str());
   
   a = hash->getDouble("noise", 20);
}

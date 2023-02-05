#ifndef _SDL_UTILITY_H_
#define _SDL_UTILITY_H_

#include <SDL/SDL.h>
#include <png.h>
#include "Color.h"

void setPixel(SDL_Surface* s, int x, int y, const Color& color);
void setPixel(SDL_Surface* s, int x, int y, Uint8 r, Uint8 g, Uint8 b);
void setPixel(SDL_Surface* s, int x, int y, Uint32 p);

Uint32 get_pixel(SDL_Surface *surface, int x, int y);

SDL_Surface* createSurface(const SDL_Rect& rect);
SDL_Surface* createSurface(int w, int h);

void user_error_fn(png_structp ctx, png_const_charp str);
void user_warning_fn(png_structp ctx, png_const_charp str);

int png_colortype_from_surface(SDL_Surface *surface);

void setBlendColor(SDL_Surface *surf, int x, int y, const RGBAColor& c);
void setBlendColor(SDL_Surface* surf, Uint32* pixel, const RGBAColor& c);
void setBlendColor(SDL_Surface * surf, int x, int y, const RGBAColor& c, int weight);

void IMG_SavePNG_RW(SDL_Surface *face, SDL_RWops *src);
void saveImage(SDL_Surface *surface, const char *file);

#endif

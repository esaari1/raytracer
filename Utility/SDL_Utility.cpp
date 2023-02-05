#include "SDL_Utility.h"

void setPixel(SDL_Surface* s, int x, int y, const Color& color) {
   setPixel(s, x, y, SDL_MapRGBA(s->format, color.getRed(), color.getGreen(), color.getBlue(), 255));
}

void setPixel(SDL_Surface* s, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
   setPixel(s, x, y, SDL_MapRGBA(s->format, r, g, b, 255));
}

void setPixel(SDL_Surface* s, int x, int y, Uint32 pixel) {
   int bpp = s->format->BytesPerPixel;
   /* Here p is the address to the pixel we want to set */
   Uint8 *p = (Uint8 *)s->pixels + y * s->pitch + x * bpp;
   *(Uint32 *)p = pixel;
}

Uint32 get_pixel(SDL_Surface *surface, int x, int y) {
   int bpp = surface->format->BytesPerPixel;
   /* Here p is the address to the pixel we want to retrieve */
   Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

   switch(bpp) {
   case 1: return *p;

   case 2: return *(Uint16 *)p;

   case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
         return p[0] << 16 | p[1] << 8 | p[2];
      }
      return p[0] | p[1] << 8 | p[2] << 16;

   case 4: return *(Uint32 *)p;

   default: return 0;       /* shouldn't happen, but avoids warnings */
   }
}

SDL_Surface* createSurface(const SDL_Rect& rect) {
   return createSurface(rect.w, rect.h);
}

SDL_Surface* createSurface(int w, int h) {
   SDL_Surface *surface;
   Uint32 rmask, gmask, bmask, amask;

   /* SDL interprets each pixel as a 32-bit number, so our masks must depend
    on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
   rmask = 0xff000000;
   gmask = 0x00ff0000;
   bmask = 0x0000ff00;
   amask = 0x000000ff;
#else
   rmask = 0x000000ff;
   gmask = 0x0000ff00;
   bmask = 0x00ff0000;
   amask = 0xff000000;
#endif

   surface = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32, rmask, gmask, bmask, amask);
   if(surface == NULL) {
      fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
      exit(1);
   }

   return surface;
}

void user_error_fn(png_structp ctx, png_const_charp str) {
   printf("libpng error %s\n", str);
}

void user_warning_fn(png_structp ctx, png_const_charp str) {
   printf("libpng warning %s\n", str);
}

int png_colortype_from_surface(SDL_Surface *surface) {
   int colortype = PNG_COLOR_MASK_COLOR; /* grayscale not supported */

   if (surface->format->palette)
      colortype |= PNG_COLOR_MASK_PALETTE;
   else if (surface->format->Amask)
      colortype |= PNG_COLOR_MASK_ALPHA;

   return colortype;
}

void setBlendColor(SDL_Surface *surf, int x, int y, const RGBAColor& c) {
   SDL_Rect rect = surf->clip_rect;
   if(x < 0 || x >= rect.w) {
      return;
   }
   if(y < 0 || y >= rect.h) {
      return;
   }
   Uint32 *pixel = (Uint32 *) surf->pixels + y * surf->pitch / 4 + x;
   setBlendColor(surf, pixel, c);
}

void setBlendColor(SDL_Surface* surf, Uint32* pixel, const RGBAColor& c) {
   Uint8 r, g, b, a;
   SDL_GetRGBA(*pixel, surf->format, &r, &g, &b, &a);
   float dr = r / 255.f;
   float dg = g / 255.f;
   float db = b / 255.f;
   float da = a / 255.f;

   float sa = c.alpha;

   float ao = sa + da * (1.f - sa);
   float ro = (c.red * sa + dr * da * (1.f - sa)) / ao;
   float go = (c.green * sa + dg * da * (1.f - sa)) / ao;
   float bo = (c.blue * sa + db * da * (1.f - sa)) / ao;

   r = ro * 255;
   g = go * 255;
   b = bo * 255;
   a = ao * 255;

   *pixel = SDL_MapRGBA(surf->format, r, g, b, a);
}

void setBlendColor(SDL_Surface * surf, int x, int y, const RGBAColor& c, int weight) {
   int alpha = ((c.getAlpha() * weight) >> 8);
   RGBAColor color(c.red, c.green, c.blue, alpha / 255.f);
   setBlendColor(surf, x, y, color);
}

static void png_write_data(png_structp ctx, png_bytep area, png_size_t size)
{
	SDL_RWops *src;
	
	src = (SDL_RWops *)png_get_io_ptr(ctx);
	SDL_RWwrite(src, area, size, 1);
}

static void png_io_flush(png_structp ctx)
{
	SDL_RWops *src;
	
	src = (SDL_RWops *)png_get_io_ptr(ctx);
	/* how do I flush src? */
}

void IMG_SavePNG_RW(SDL_Surface *face, SDL_RWops *src) {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	const int rmask = 0x00ff0000;
	const int gmask = 0x0000ff00;
	const int bmask = 0x000000ff;
	const int amask = 0x00000000;
#else
   const int rmask = 0x000000ff;
	const int gmask = 0x0000ff00;
	const int bmask = 0x00ff0000;
	const int amask = 0x00000000;
#endif
   
   SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, face->w, face->h, 24,
                                               rmask, gmask, bmask, amask);

   if (surface) {
      SDL_BlitSurface(face, NULL, surface, NULL);
      SDL_LockSurface(surface);

      png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, user_error_fn, user_warning_fn);
      if (!png_ptr) {
         printf("Couldn't allocate memory for PNG file");
      }
      else {
         /* Allocate/initialize the image information data.  REQUIRED */
         png_infop info_ptr = png_create_info_struct(png_ptr);
         if (!info_ptr) {
            printf("Couldn't create image information for PNG file");
         }
         else {
            png_bytep *row_pointers = 0;

            /* Set error handling. */
            if (setjmp(png_jmpbuf(png_ptr))) {
               printf("Error writing the PNG file");
            }
            else {
               png_set_write_fn(png_ptr, src, png_write_data, png_io_flush);
               /* Set the image information here.  Width and height are up to 2^31,
                * bit_depth is one of 1, 2, 4, 8, or 16, but valid values also depend on
                * the color_type selected. color_type is one of PNG_COLOR_TYPE_GRAY,
                * PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
                * or PNG_COLOR_TYPE_RGB_ALPHA.  interlace is either PNG_INTERLACE_NONE or
                * PNG_INTERLACE_ADAM7, and the compression_type and filter_type MUST
                * currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE. REQUIRED
                */
               int colortype = png_colortype_from_surface(surface);
               png_set_IHDR(png_ptr, info_ptr, surface->w, surface->h, 8,
                            colortype, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
               
               /* Write the file header information.  REQUIRED */
               png_write_info(png_ptr, info_ptr);
               
               /* pack pixels into bytes */
               png_set_packing(png_ptr);
               
               /* Create the array of pointers to image data */
               row_pointers = (png_bytep*) malloc(sizeof(png_bytep)*surface->h);
               if (!row_pointers) {
                  printf("Couldn't allocate PNG row pointers");
               }
               else {
                  for (int i = 0; i < surface->h; i++) {
                     row_pointers[i] = (png_bytep)(Uint8 *)surface->pixels + i*surface->pitch;
                  }
                  
                  /* write out the entire image data in one call */
                  png_write_image(png_ptr, row_pointers);
                  png_write_end(png_ptr, info_ptr);
               }
            }
            if (row_pointers) {
               free(row_pointers);
            }
            
            png_destroy_info_struct(png_ptr, &info_ptr);
         }
         png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
      }
      
      SDL_UnlockSurface(surface);
      SDL_FreeSurface(surface);
   }
}

void saveImage(SDL_Surface *surface, const char *file) {
   SDL_RWops *out = SDL_RWFromFile(file, "wb");
   if(!out) {
      return ;
   }

   IMG_SavePNG_RW(surface, out);
   SDL_RWclose(out);
}

#ifndef __UTIL_GRAPHICS_H__
#define __UTIL_GRAPHICS_H__

#include <SDL.h>

Uint32 get_pixel32( SDL_Surface *surface, int x, int y );
void put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel );


Uint32 getpixel(SDL_Surface *surface, int x, int y);
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);


#endif /* __UTIL_GRAPHICS_H__ */

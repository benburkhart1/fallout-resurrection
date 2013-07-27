#ifndef __RIX_HANDLER_H__
#define __RIX_HANDLER_H__

#include <SDL.h>

struct dat_file_file;

struct rix_file
{
	char identifier[4];

	Uint16 width;
	Uint16 height;
	
	Uint8 unknown1;
	
	Uint8 palette[256*3];
};

void rix_draw(SDL_Surface *screen, struct dat_file_file *dff);

#endif /* __RIX_HANDLER_H__ */

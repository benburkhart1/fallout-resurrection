/* RIX3 files are the files Fallout uses for splash screens */
/* It's relatively simple, it has a dictionary and then references for width*height */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL.h>

#include "rix_handler.h"
#include "dat_file.h"
#include "util-graphics.h"
#include "util.h"

void rix_draw(SDL_Surface *screen, struct dat_file_file *dff)
{
	int x;
	int y;
	int fi = 0;
	int i;
	struct rix_file *rf;
	Uint32 color;
	unsigned char r;
	unsigned char g;
	unsigned char b;

	rf = tmalloc0(sizeof(struct rix_file));

	/* Make sure the dff is a rix file */
	if (strncmp(dff->data, "RIX", 3))
	{
		printf("This isn't a RIX3 file\n");
		exit(EXIT_FAILURE);
	}

	memcpy(rf->identifier, dff->data, 4);
	fi+=4;

	memcpy(&rf->width, &dff->data[fi], 2);
	fi+=2;	

	memcpy(&rf->height, &dff->data[fi], 2);
	fi+=2;	

	rf->unknown1 = dff->data[fi];
	fi++;
	
	memcpy(rf->palette, &dff->data[fi], 256*3);
	fi+=256*3;
	
	SDL_LockSurface(screen);

	/* Left to right, then top to bottom */
	for (y = 0; y < rf->height; y++)
	{
		for (x = 0; x < rf->width; x++)
		{
			r = (rf->palette[dff->data[fi]*3]<<2);
			g = (rf->palette[dff->data[fi]*3+1]<<2);
			b = (rf->palette[dff->data[fi]*3+2]<<2);

			color = SDL_MapRGB(screen->format, r, g, b);
//			put_pixel32(screen, x, y, color);
			putpixel(screen, x, y, color);
			fi++;
		}
	}

	SDL_UnlockSurface(screen);
	
	printf("%dx%d\n",rf->width,rf->height);
	
	return;
}

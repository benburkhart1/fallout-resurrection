#ifndef __FRM_HANDLER_H__
#define __FRM_HANDLER_H__

#include <SDL.h>

struct dat_file_file;

struct frm_file
{
	/* Version number of the FRM file format */
	Uint32 version;

	/* Frames per second if an animation */
	Uint16 fps;
	
	/* Frame of the animation on which actions aoccur (shot, open doors, etc) */
	Uint16 action_frame;

	/*  */
	Uint16 num_frames_per_direction;

	/*  */
	Int16 required_x_shift;
	Int16 required_y_shift;
};

//void frm_draw(SDL_Surface *screen, struct dat_file_file *dff, );

#endif /* __RIX_HANDLER_H__ */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "util.h"
#include "dat_file.h"
#include "dat1_file.h"
#include "rix_handler.h"
#include "SDL.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_DEPTH 32

int main(int argc, char *argv[]) 
{
	struct dat1_file *df;
	struct dat_file_file *dff;
	FILE *fp;
	SDL_Surface *screen;
	SDL_Event event;
	int        quit = 0; 


	df = dat1_file_new_from_filename("MASTER.DAT");

	if (df != NULL)
	{
		printf("DF is not NULL\n");
	}

	dff = dat_file_file_from_dat1_file(df, "SPLASH1.RIX");

	/* Save a copy in file.tmp *
	if ((fp = fopen("file.tmp","w+")) == NULL)
	{
		printf("Error opening tmp file\n");
		exit(EXIT_FAILURE);
	}

	fwrite(dff->data, dff->file_size, 1, fp);
	fclose(fp);

	dat1_file_free(df);
	dat_file_file_free(dff);	
	*/

	/* Initialize SDL */
	SDL_Init(SDL_INIT_VIDEO);

	/* Initialize the screen / window */
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH, SDL_HWSURFACE |  SDL_HWPALETTE);

	rix_draw(screen, dff);	

	while(!quit)
	{
		/* check the message queue for an event */
		if (SDL_PollEvent(&event))
		{
			/* if an event was found */
			switch (event.type)
			{
				/* check to see if the window was closed via the "x button" */
				case SDL_QUIT:
				/* set the quit flag to true */
					quit = 1;
					break;

				/* check the keyboard to see if the ESC key was pressed */
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_FIRST: break;
						case SDLK_BACKSPACE: break;
						case SDLK_TAB: break;
						case SDLK_CLEAR: break;
						case SDLK_RETURN: break;
						case SDLK_PAUSE: break;
						case SDLK_SPACE: break;
						case SDLK_EXCLAIM: break;
						case SDLK_QUOTEDBL: break;
						case SDLK_HASH: break;
						case SDLK_DOLLAR: break;
						case SDLK_AMPERSAND: break;
						case SDLK_QUOTE: break;
						case SDLK_LEFTPAREN: break;
						case SDLK_RIGHTPAREN: break;
						case SDLK_ASTERISK: break;
						case SDLK_PLUS: break;
						case SDLK_COMMA: break;
						case SDLK_MINUS: break;
						case SDLK_PERIOD: break;
						case SDLK_SLASH: break;
						case SDLK_0: break;
						case SDLK_1: break;
						case SDLK_2: break;
						case SDLK_3: break;
						case SDLK_4: break;
						case SDLK_5: break;
						case SDLK_6: break;
						case SDLK_7: break;
						case SDLK_8: break;
						case SDLK_9: break;
						case SDLK_COLON: break;
						case SDLK_SEMICOLON: break;
						case SDLK_LESS: break;
						case SDLK_EQUALS: break;
						case SDLK_GREATER: break;
						case SDLK_QUESTION: break;
						case SDLK_AT: break;
						case SDLK_LEFTBRACKET: break;
						case SDLK_BACKSLASH: break;
						case SDLK_RIGHTBRACKET: break;
						case SDLK_CARET: break;
						case SDLK_UNDERSCORE: break;
						case SDLK_BACKQUOTE: break;
						case SDLK_a: break;
						case SDLK_b: break;
						case SDLK_c: break;
						case SDLK_d: break;
						case SDLK_e: break;
						case SDLK_f: break;
						case SDLK_g: break;
						case SDLK_h: break;
						case SDLK_i: break;
						case SDLK_j: break;
						case SDLK_k: break;
						case SDLK_l: break;
						case SDLK_m: break;
						case SDLK_n: break;
						case SDLK_o: break;
						case SDLK_p: break;
						case SDLK_r: break;
						case SDLK_s: break;
						case SDLK_t: break;
						case SDLK_u: break;
						case SDLK_v: break;
						case SDLK_w: break;
						case SDLK_x: break;
						case SDLK_y: break;
						case SDLK_z: break;
						case SDLK_DELETE: break;
						case SDLK_WORLD_0: break;
						case SDLK_WORLD_1: break;
						case SDLK_WORLD_2: break;
						case SDLK_WORLD_3: break;
						case SDLK_WORLD_4: break;
						case SDLK_WORLD_5: break;
						case SDLK_WORLD_6: break;
						case SDLK_WORLD_7: break;
						case SDLK_WORLD_8: break;
						case SDLK_WORLD_9: break;
						case SDLK_WORLD_10: break;
						case SDLK_WORLD_11: break;
						case SDLK_WORLD_12: break;
						case SDLK_WORLD_13: break;
						case SDLK_WORLD_14: break;
						case SDLK_WORLD_15: break;
						case SDLK_WORLD_16: break;
						case SDLK_WORLD_17: break;
						case SDLK_WORLD_18: break;
						case SDLK_WORLD_19: break;
						case SDLK_WORLD_20: break;
						case SDLK_WORLD_21: break;
						case SDLK_WORLD_22: break;
						case SDLK_WORLD_23: break;
						case SDLK_WORLD_24: break;
						case SDLK_WORLD_25: break;
						case SDLK_WORLD_26: break;
						case SDLK_WORLD_27: break;
						case SDLK_WORLD_28: break;
						case SDLK_WORLD_29: break;
						case SDLK_WORLD_30: break;
						case SDLK_WORLD_31: break;
						case SDLK_WORLD_32: break;
						case SDLK_WORLD_33: break;
						case SDLK_WORLD_34: break;
						case SDLK_WORLD_35: break;
						case SDLK_WORLD_36: break;
						case SDLK_WORLD_37: break;
						case SDLK_WORLD_38: break;
						case SDLK_WORLD_39: break;
						case SDLK_WORLD_40: break;
						case SDLK_WORLD_41: break;
						case SDLK_WORLD_42: break;
						case SDLK_WORLD_43: break;
						case SDLK_WORLD_44: break;
						case SDLK_WORLD_45: break;
						case SDLK_WORLD_46: break;
						case SDLK_WORLD_47: break;
						case SDLK_WORLD_48: break;
						case SDLK_WORLD_49: break;
						case SDLK_WORLD_50: break;
						case SDLK_WORLD_51: break;
						case SDLK_WORLD_52: break;
						case SDLK_WORLD_53: break;
						case SDLK_WORLD_54: break;
						case SDLK_WORLD_55: break;
						case SDLK_WORLD_56: break;
						case SDLK_WORLD_57: break;
						case SDLK_WORLD_58: break;
						case SDLK_WORLD_59: break;
						case SDLK_WORLD_60: break;
						case SDLK_WORLD_61: break;
						case SDLK_WORLD_62: break;
						case SDLK_WORLD_63: break;
						case SDLK_WORLD_64: break;
						case SDLK_WORLD_65: break;
						case SDLK_WORLD_66: break;
						case SDLK_WORLD_67: break;
						case SDLK_WORLD_68: break;
						case SDLK_WORLD_69: break;
						case SDLK_WORLD_70: break;
						case SDLK_WORLD_71: break;
						case SDLK_WORLD_72: break;
						case SDLK_WORLD_73: break;
						case SDLK_WORLD_74: break;
						case SDLK_WORLD_75: break;
						case SDLK_WORLD_76: break;
						case SDLK_WORLD_77: break;
						case SDLK_WORLD_78: break;
						case SDLK_WORLD_79: break;
						case SDLK_WORLD_80: break;
						case SDLK_WORLD_81: break;
						case SDLK_WORLD_82: break;
						case SDLK_WORLD_83: break;
						case SDLK_WORLD_84: break;
						case SDLK_WORLD_85: break;
						case SDLK_WORLD_86: break;
						case SDLK_WORLD_87: break;
						case SDLK_WORLD_88: break;
						case SDLK_WORLD_89: break;
						case SDLK_WORLD_90: break;
						case SDLK_WORLD_91: break;
						case SDLK_WORLD_92: break;
						case SDLK_WORLD_93: break;
						case SDLK_WORLD_94: break;
						case SDLK_WORLD_95: break;
						case SDLK_KP0: break;
						case SDLK_KP1: break;
						case SDLK_KP2: break;
						case SDLK_KP3: break;
						case SDLK_KP4: break;
						case SDLK_KP5: break;
						case SDLK_KP6: break;
						case SDLK_KP7: break;
						case SDLK_KP8: break;
						case SDLK_KP9: break;
						case SDLK_KP_PERIOD: break;
						case SDLK_KP_DIVIDE: break;
						case SDLK_KP_MULTIPLY: break;
						case SDLK_KP_MINUS: break;
						case SDLK_KP_PLUS: break;
						case SDLK_KP_ENTER: break;
						case SDLK_KP_EQUALS: break;
						case SDLK_UP: break;
						case SDLK_DOWN: break;
						case SDLK_RIGHT: break;
						case SDLK_LEFT: break;
						case SDLK_INSERT: break;
						case SDLK_HOME: break;
						case SDLK_END: break;
						case SDLK_PAGEUP: break;
						case SDLK_PAGEDOWN: break;
						case SDLK_F1: break;
						case SDLK_F2: break;
						case SDLK_F3: break;
						case SDLK_F4: break;
						case SDLK_F5: break;
						case SDLK_F6: break;
						case SDLK_F7: break;
						case SDLK_F8: break;
						case SDLK_F9: break;
						case SDLK_F10: break;
						case SDLK_F11: break;
						case SDLK_F12: break;
						case SDLK_F13: break;
						case SDLK_F14: break;
						case SDLK_F15: break;
						case SDLK_NUMLOCK: break;
						case SDLK_CAPSLOCK: break;
						case SDLK_SCROLLOCK: break;
						case SDLK_RSHIFT: break;
						case SDLK_LSHIFT: break;
						case SDLK_RCTRL: break;
						case SDLK_LCTRL: break;
						case SDLK_RALT: break;
						case SDLK_LALT: break;
						case SDLK_RMETA: break;
						case SDLK_LMETA: break;
						case SDLK_LSUPER: break;
						case SDLK_RSUPER: break;
						case SDLK_MODE: break;
						case SDLK_COMPOSE: break;
						case SDLK_HELP: break;
						case SDLK_PRINT: break;
						case SDLK_SYSREQ: break;
						case SDLK_BREAK: break;
						case SDLK_MENU: break;
						case SDLK_POWER: break;
						case SDLK_EURO: break;
						case SDLK_UNDO: break;
						case SDLK_LAST: break;
						case SDLK_ESCAPE:
						case SDLK_q:
							/* set our quit flag to true */
							quit = 1;
							break;
					}
			}
		}

		SDL_Flip(screen);
	}

	SDL_Quit();
}

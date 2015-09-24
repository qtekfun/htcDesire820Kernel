

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"

static void quit(int rc)
{
	SDL_Quit();
	exit(rc);
}

static void print_modifiers(void)
{
	int mod;
	printf(" modifiers:");
	mod = SDL_GetModState();
	if(!mod) {
		printf(" (none)");
		return;
	}
	if(mod & KMOD_LSHIFT)
		printf(" LSHIFT");
	if(mod & KMOD_RSHIFT)
		printf(" RSHIFT");
	if(mod & KMOD_LCTRL)
		printf(" LCTRL");
	if(mod & KMOD_RCTRL)
		printf(" RCTRL");
	if(mod & KMOD_LALT)
		printf(" LALT");
	if(mod & KMOD_RALT)
		printf(" RALT");
	if(mod & KMOD_LMETA)
		printf(" LMETA");
	if(mod & KMOD_RMETA)
		printf(" RMETA");
	if(mod & KMOD_NUM)
		printf(" NUM");
	if(mod & KMOD_CAPS)
		printf(" CAPS");
	if(mod & KMOD_MODE)
		printf(" MODE");
}

static void PrintKey(SDL_keysym *sym, int pressed)
{
	
	if ( sym->sym ) {
		printf("Key %s:  %d-%s ", pressed ?  "pressed" : "released",
					sym->sym, SDL_GetKeyName(sym->sym));
	} else {
		printf("Unknown Key (scancode = %d) %s ", sym->scancode,
					pressed ?  "pressed" : "released");
	}

	
	if ( sym->unicode ) {
		
		if ( sym->unicode < ' ' ) {
			printf(" (^%c)", sym->unicode+'@');
		} else {
#ifdef UNICODE
			printf(" (%c)", sym->unicode);
#else
			
			if ( !(sym->unicode & 0xFF00) )
				printf(" (%c)", sym->unicode);
			else
				printf(" (0x%X)", sym->unicode);
#endif
		}
	}
	print_modifiers();
	printf("\n");
}

int main(int argc, char *argv[])
{
	SDL_Event event;
	int done;
	Uint32 videoflags;

	
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
		return(1);
	}

	videoflags = SDL_SWSURFACE;
	while( argc > 1 ) {
		--argc;
		if ( argv[argc] && !strcmp(argv[argc], "-fullscreen") ) {
			videoflags |= SDL_FULLSCREEN;
		} else {
			fprintf(stderr, "Usage: %s [-fullscreen]\n", argv[0]);
			quit(1);
		}
	}

	
	if ( SDL_SetVideoMode(640, 480, 0, videoflags) == NULL ) {
		fprintf(stderr, "Couldn't set 640x480 video mode: %s\n",
							SDL_GetError());
		quit(2);
	}

	
	SDL_EnableUNICODE(1);

	
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,
	                    SDL_DEFAULT_REPEAT_INTERVAL);

	
	done = 0;
	while ( !done ) {
		
		SDL_WaitEvent(&event);
		switch (event.type) {
			case SDL_KEYDOWN:
				PrintKey(&event.key.keysym, 1);
				break;
			case SDL_KEYUP:
				PrintKey(&event.key.keysym, 0);
				break;
			case SDL_MOUSEBUTTONDOWN:
				
			case SDL_QUIT:
				done = 1;
				break;
			default:
				break;
		}
	}

	SDL_Quit();
	return(0);
}

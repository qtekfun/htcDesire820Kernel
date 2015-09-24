

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "SDL.h"
#include "SDL_thread.h"

static int done = 0;

static int visible = 1;

static void quit(int rc)
{
	SDL_Quit();
	exit(rc);
}

SDL_Surface *LoadIconSurface(char *file, Uint8 **maskp)
{
	SDL_Surface *icon;
	Uint8       *pixels;
	Uint8       *mask;
	int          mlen, i;

	*maskp = NULL;

	
	icon = SDL_LoadBMP(file);
	if ( icon == NULL ) {
		fprintf(stderr, "Couldn't load %s: %s\n", file, SDL_GetError());
		return(NULL);
	}

	
	if ( (icon->w%8) != 0 ) {
		fprintf(stderr, "Icon width must be a multiple of 8!\n");
		SDL_FreeSurface(icon);
		return(NULL);
	}
	if ( icon->format->palette == NULL ) {
		fprintf(stderr, "Icon must have a palette!\n");
		SDL_FreeSurface(icon);
		return(NULL);
	}

	
	SDL_SetColorKey(icon, SDL_SRCCOLORKEY, *((Uint8 *)icon->pixels));

	
	pixels = (Uint8 *)icon->pixels;
	printf("Transparent pixel: (%d,%d,%d)\n",
				icon->format->palette->colors[*pixels].r,
				icon->format->palette->colors[*pixels].g,
				icon->format->palette->colors[*pixels].b);
	mlen = icon->w*icon->h;
	mask = (Uint8 *)malloc(mlen/8);
	if ( mask == NULL ) {
		fprintf(stderr, "Out of memory!\n");
		SDL_FreeSurface(icon);
		return(NULL);
	}
	memset(mask, 0, mlen/8);
	for ( i=0; i<mlen; ) {
		if ( pixels[i] != *pixels )
			mask[i/8] |= 0x01;
		++i;
		if ( (i%8) != 0 )
			mask[i/8] <<= 1;
	}
	*maskp = mask;
	return(icon);
}

int SDLCALL FilterEvents(const SDL_Event *event)
{
	static int reallyquit = 0;

	switch (event->type) {

		case SDL_ACTIVEEVENT:
			
			printf("App %s ",
				event->active.gain ? "gained" : "lost");
			if ( event->active.state & SDL_APPACTIVE )
				printf("active ");
			if ( event->active.state & SDL_APPMOUSEFOCUS )
				printf("mouse ");
			if ( event->active.state & SDL_APPINPUTFOCUS )
				printf("input ");
			printf("focus\n");

			
			if ( event->active.state & SDL_APPACTIVE ) {
				printf("App has been %s\n",
					event->active.gain ?
						 "restored" : "iconified");
			}
			return(0);

		
		case SDL_QUIT:
			if ( ! reallyquit ) {
				reallyquit = 1;
				printf("Quit requested\n");
				return(0);
			}
			printf("Quit demanded\n");
			return(1);

		
		case SDL_MOUSEMOTION:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			return(1);

		
		default:
			return(0);
	}
}

int SDLCALL HandleMouse(void *unused)
{
	SDL_Event events[10];
	int i, found;
	Uint32 mask;

	
	mask = (SDL_MOUSEMOTIONMASK|SDL_MOUSEBUTTONDOWNMASK|SDL_MOUSEBUTTONUPMASK);
	while ( ! done ) {
		found = SDL_PeepEvents(events, 10, SDL_GETEVENT, mask);
		for ( i=0; i<found; ++i ) {
			switch(events[i].type) {
				
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
					if ( events[i].button.state == SDL_PRESSED ) {
						visible = !visible;
						SDL_ShowCursor(visible);
					}
					printf("Mouse button %d has been %s\n",
						events[i].button.button,
						(events[i].button.state == SDL_PRESSED) ?
						"pressed" : "released");
					break;
				
				case SDL_MOUSEMOTION:
					printf("Mouse relative motion: {%d,%d}\n",
							events[i].motion.xrel, events[i].motion.yrel);
					break;
			}
		}
		
		SDL_Delay(20);
	}
	return(0);
}

int SDLCALL HandleKeyboard(void *unused)
{
	SDL_Event events[10];
	int i, found;
	Uint32 mask;

	
	mask = (SDL_KEYDOWNMASK|SDL_KEYUPMASK);
	while ( ! done ) {
		found = SDL_PeepEvents(events, 10, SDL_GETEVENT, mask);
		for ( i=0; i<found; ++i ) {
			switch(events[i].type) {
			    
			    case SDL_KEYDOWN:
			    case SDL_KEYUP:
			    	printf("Key '%c' (keysym==%d) has been %s\n",
						events[i].key.keysym.unicode,
						(int) events[i].key.keysym.sym,
					(events[i].key.state == SDL_PRESSED) ?
						"pressed" : "released");

			    	
			    	if ( events[i].key.keysym.sym == SDLK_ESCAPE ) {
			    		done = 1;
			    	}

					
					if (events[i].key.state == SDL_PRESSED)
						break;

			    	if ( events[i].key.keysym.sym == SDLK_f ) {
						int rc = 0;
						printf("attempting to toggle fullscreen...\n");
						rc = SDL_WM_ToggleFullScreen(SDL_GetVideoSurface());
                        printf("SDL_WM_ToggleFullScreen returned %d.\n", rc);
			    	}

			    	if ( events[i].key.keysym.sym == SDLK_g ) {
						SDL_GrabMode m;
						m = SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_ON ?
								SDL_GRAB_OFF : SDL_GRAB_ON;
						printf("attempting to toggle input grab to %s...\n",
                                m == SDL_GRAB_ON ? "ON" : "OFF");
                        SDL_WM_GrabInput(m);
						printf("attempt finished.\n");
			    	}

			    	break;
			}
		}
		
		SDL_Delay(20);
	}
	return(0);
}

int main(int argc, char *argv[])
{
	SDL_Surface *screen;
	SDL_Surface *icon;
	Uint8 *icon_mask;
	int i, parsed;
	Uint8 *buffer;
	SDL_Color palette[256];
	Uint32 init_flags;
	Uint8  video_bpp;
	Uint32 video_flags;
	SDL_Thread *mouse_thread;
	SDL_Thread *keybd_thread;

	
	init_flags = SDL_INIT_VIDEO;
	video_bpp = 8;
	video_flags = SDL_SWSURFACE;
	parsed = 1;
	while ( parsed ) {
		if ( (argc >= 2) && (strcmp(argv[1], "-threaded") == 0) ) {
			init_flags |= SDL_INIT_EVENTTHREAD;
			argc -= 1;
			argv += 1;
			printf("Running with threaded events\n");
		} else
		if ( (argc >= 2) && (strcmp(argv[1], "-fullscreen") == 0) ) {
			video_flags |= SDL_FULLSCREEN;
			argc -= 1;
			argv += 1;
		} else
		if ( (argc >= 3) && (strcmp(argv[1], "-bpp") == 0) ) {
			video_bpp = atoi(argv[2]);
			argc -= 2;
			argv += 2;
		} else {
			parsed = 0;
		}
	}

	
	if ( SDL_Init(init_flags) < 0 ) {
		fprintf(stderr,
			"Couldn't initialize SDL: %s\n", SDL_GetError());
		return(1);
	}

	
	icon = LoadIconSurface("icon.bmp", &icon_mask);
	if ( icon != NULL ) {
		SDL_WM_SetIcon(icon, icon_mask);
	}
	if ( icon_mask != NULL )
		free(icon_mask);

	
	screen = SDL_SetVideoMode(640, 480, video_bpp, video_flags);
	if (  screen == NULL ) {
		fprintf(stderr, "Couldn't set 640x480x%d video mode: %s\n",
						video_bpp, SDL_GetError());
		quit(1);
	}
	printf("Running in %s mode\n", screen->flags & SDL_FULLSCREEN ?
						"fullscreen" : "windowed");

	
	SDL_EnableUNICODE(1);

	
	SDL_SetEventFilter(FilterEvents);

	
	mouse_thread = SDL_CreateThread(HandleMouse, NULL);
	keybd_thread = SDL_CreateThread(HandleKeyboard, NULL);

	
	for ( i=0; i<256; ++i ) {
		palette[i].r = 255-i;
		palette[i].g = 255-i;
		palette[i].b = 255-i;
	}
	SDL_SetColors(screen, palette, 0, 256);
	if ( SDL_LockSurface(screen) < 0 ) {
		fprintf(stderr, "Couldn't lock display surface: %s\n",
							SDL_GetError());
		quit(2);
	}
	buffer = (Uint8 *)screen->pixels;
	for ( i=0; i<screen->h; ++i ) {
		memset(buffer,(i*255)/screen->h,
				screen->w*screen->format->BytesPerPixel);
		buffer += screen->pitch;
	}
	SDL_UnlockSurface(screen);
	SDL_UpdateRect(screen, 0, 0, 0, 0);

	
	while ( ! done ) {
		if ( ! (init_flags & SDL_INIT_EVENTTHREAD) ) {
			SDL_PumpEvents(); 
		}
		if ( SDL_PeepEvents(NULL, 0, SDL_PEEKEVENT, SDL_QUITMASK) ) {
			done = 1;
		}
		
		SDL_Delay(20);
	}
	SDL_WaitThread(mouse_thread, NULL);
	SDL_WaitThread(keybd_thread, NULL);
	SDL_Quit();
	return(0);
}

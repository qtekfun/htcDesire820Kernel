

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "SDL.h"

static void quit(int rc)
{
	SDL_Quit();
	exit(rc);
}

void CalculateGamma(double gamma, Uint16 *ramp)
{
	int i, value;

	gamma = 1.0 / gamma;
	for ( i=0; i<256; ++i ) {
		value = (int)(pow((double)i/256.0, gamma)*65535.0 + 0.5);
		if ( value > 65535 ) {
			value = 65535;
		}
		ramp[i] = (Uint16)value;
	}
}

int get_video_args(char *argv[], int *w, int *h, int *bpp, Uint32 *flags)
{
	int i;

	*w = 640;
	*h = 480;
	*bpp = 0;
	*flags = SDL_SWSURFACE;

	for ( i=1; argv[i]; ++i ) {
		if ( strcmp(argv[i], "-width") == 0 ) {
			if ( argv[i+1] ) {
				*w = atoi(argv[++i]);
			}
		} else
		if ( strcmp(argv[i], "-height") == 0 ) {
			if ( argv[i+1] ) {
				*h = atoi(argv[++i]);
			}
		} else
		if ( strcmp(argv[i], "-bpp") == 0 ) {
			if ( argv[i+1] ) {
				*bpp = atoi(argv[++i]);
			}
		} else
		if ( strcmp(argv[i], "-fullscreen") == 0 ) {
			*flags |= SDL_FULLSCREEN;
		} else
		if ( strcmp(argv[i], "-hw") == 0 ) {
			*flags |= SDL_HWSURFACE;
		} else
		if ( strcmp(argv[i], "-hwpalette") == 0 ) {
			*flags |= SDL_HWPALETTE;
		} else
			break;
	}
	return i;
}

int main(int argc, char *argv[])
{
	SDL_Surface *screen;
	SDL_Surface *image;
	float gamma;
	int i;
	int w, h, bpp;
	Uint32 flags;
	Uint16 ramp[256];
	Uint16 red_ramp[256];
	Uint32 then, timeout;

	
	argv += get_video_args(argv, &w, &h, &bpp, &flags);

	
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr,
			"Couldn't initialize SDL: %s\n", SDL_GetError());
		return(1);
	}

	
	screen = SDL_SetVideoMode(w, h, bpp, flags | SDL_HWPALETTE);
	if ( screen == NULL ) {
		fprintf(stderr, "Couldn't set %dx%d video mode: %s\n",
						w, h, SDL_GetError());
		quit(1);
	}

	
	SDL_WM_SetCaption("SDL gamma test", "testgamma");

	
	gamma = 1.0f;
	if ( *argv ) {
		gamma = (float)atof(*argv);
	}
	if ( SDL_SetGamma(gamma, gamma, gamma) < 0 ) {
		fprintf(stderr, "Unable to set gamma: %s\n", SDL_GetError());
		quit(1);
	}

#if 0 
	
	float real[3];
	if ( SDL_GetGamma(&real[0], &real[1], &real[2]) < 0 ) {
		printf("Couldn't get gamma: %s\n", SDL_GetError());
	} else {
		printf("Set gamma values: R=%2.2f, G=%2.2f, B=%2.2f\n",
			real[0], real[1], real[2]);
	}
#endif

	
	image = SDL_LoadBMP("sample.bmp");
	if ( image ) {
		SDL_Rect dst;

		dst.x = (screen->w - image->w)/2;
		dst.y = (screen->h - image->h)/2;
		dst.w = image->w;
		dst.h = image->h;
		SDL_BlitSurface(image, NULL, screen, &dst);
		SDL_UpdateRects(screen, 1, &dst);
	}

	
	then = SDL_GetTicks();
	timeout = (5*1000);
	while ( (SDL_GetTicks()-then) < timeout ) {
		SDL_Event event;

		while ( SDL_PollEvent(&event) ) {
			switch (event.type) {
			    case SDL_QUIT:	
				timeout = 0;
				break;
			    case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				    case SDLK_SPACE:	
					timeout += (5*1000);
					break;
				    case SDLK_UP:
					gamma += 0.2f;
					SDL_SetGamma(gamma, gamma, gamma);
					break;
				    case SDLK_DOWN:
					gamma -= 0.2f;
					SDL_SetGamma(gamma, gamma, gamma);
					break;
				    case SDLK_ESCAPE:
					timeout = 0;
					break;
				    default:
					break;
				}
				break;
			}
		}
	}

	
	while ( gamma < 10.0 ) {
		
		gamma += 0.1f;
		CalculateGamma(gamma, red_ramp);
		CalculateGamma(1.0/gamma, ramp);
		SDL_SetGammaRamp(red_ramp, ramp, ramp);
	}
	
	memset(red_ramp, 255, sizeof(red_ramp));
	memset(ramp, 0, sizeof(ramp));
	SDL_SetGammaRamp(red_ramp, ramp, ramp);

	
	for ( i=(red_ramp[0] >> 8); i >= 0; --i ) {
		memset(red_ramp, i, sizeof(red_ramp));
		SDL_SetGammaRamp(red_ramp, NULL, NULL);
	}
	SDL_Delay(1*1000);

	SDL_Quit();
	return(0);
}

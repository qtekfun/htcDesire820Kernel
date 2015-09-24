
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#include "SDL.h"

#define NUM_SPRITES	100
#define MAX_SPEED 	1

SDL_Surface *sprite;
int numsprites;
SDL_Rect *sprite_rects;
SDL_Rect *positions;
SDL_Rect *velocities;
int sprites_visible;
int debug_flip;
Uint16 sprite_w, sprite_h;

static void quit(int rc)
{
	SDL_Quit();
	exit(rc);
}

int LoadSprite(char *file)
{
	SDL_Surface *temp;

	
	sprite = SDL_LoadBMP(file);
	if ( sprite == NULL ) {
		fprintf(stderr, "Couldn't load %s: %s", file, SDL_GetError());
		return(-1);
	}

	
	if ( sprite->format->palette ) {
		SDL_SetColorKey(sprite, (SDL_SRCCOLORKEY|SDL_RLEACCEL),
						*(Uint8 *)sprite->pixels);
	}

	
	temp = SDL_DisplayFormat(sprite);
	SDL_FreeSurface(sprite);
	if ( temp == NULL ) {
		fprintf(stderr, "Couldn't convert background: %s\n",
							SDL_GetError());
		return(-1);
	}
	sprite = temp;

	
	return(0);
}

void MoveSprites(SDL_Surface *screen, Uint32 background)
{
	int i, nupdates;
	SDL_Rect area, *position, *velocity;

	nupdates = 0;
	
	if ( sprites_visible ) {
		SDL_FillRect(screen, NULL, background);
	}

	
	for ( i=0; i<numsprites; ++i ) {
		position = &positions[i];
		velocity = &velocities[i];
		position->x += velocity->x;
		if ( (position->x < 0) || (position->x >= (screen->w - sprite_w)) ) {
			velocity->x = -velocity->x;
			position->x += velocity->x;
		}
		position->y += velocity->y;
		if ( (position->y < 0) || (position->y >= (screen->h - sprite_w)) ) {
			velocity->y = -velocity->y;
			position->y += velocity->y;
		}

		
		area = *position;
		SDL_BlitSurface(sprite, NULL, screen, &area);
		sprite_rects[nupdates++] = area;
	}

	if (debug_flip) {
		if ( (screen->flags & SDL_DOUBLEBUF) == SDL_DOUBLEBUF ) {
			static int t = 0;

			Uint32 color = SDL_MapRGB (screen->format, 255, 0, 0);
			SDL_Rect r;
			r.x = (sin((float)t * 2 * 3.1459) + 1.0) / 2.0 * (screen->w-20);
			r.y = 0;
			r.w = 20;
			r.h = screen->h;

			SDL_FillRect (screen, &r, color);
			t+=2;
		}
	}

	
	if ( (screen->flags & SDL_DOUBLEBUF) == SDL_DOUBLEBUF ) {
		SDL_Flip(screen);
	} else {
		SDL_UpdateRects(screen, nupdates, sprite_rects);
	}
	sprites_visible = 1;
}

Uint32 FastestFlags(Uint32 flags, int width, int height, int bpp)
{
	const SDL_VideoInfo *info;

	
	flags |= SDL_FULLSCREEN;

	
	info = SDL_GetVideoInfo();
	if ( info->blit_hw_CC && info->blit_fill ) {
		
		flags |= SDL_HWSURFACE;
	}
	if ( (flags & SDL_HWSURFACE) == SDL_HWSURFACE ) {
		if ( info->video_mem*1024 > (height*width*bpp/8) ) {
			flags |= SDL_DOUBLEBUF;
		} else {
			flags &= ~SDL_HWSURFACE;
		}
	}

	
	return(flags);
}

int main(int argc, char *argv[])
{
	SDL_Surface *screen;
	Uint8 *mem;
	int width, height;
	Uint8  video_bpp;
	Uint32 videoflags;
	Uint32 background;
	int    i, done;
	SDL_Event event;
	Uint32 then, now, frames;

	
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
		return(1);
	}

	numsprites = NUM_SPRITES;
	videoflags = SDL_SWSURFACE|SDL_ANYFORMAT;
	width = 640;
	height = 480;
	video_bpp = 8;
	debug_flip = 0;
	while ( argc > 1 ) {
		--argc;
		if ( strcmp(argv[argc-1], "-width") == 0 ) {
			width = atoi(argv[argc]);
			--argc;
		} else
		if ( strcmp(argv[argc-1], "-height") == 0 ) {
			height = atoi(argv[argc]);
			--argc;
		} else
		if ( strcmp(argv[argc-1], "-bpp") == 0 ) {
			video_bpp = atoi(argv[argc]);
			videoflags &= ~SDL_ANYFORMAT;
			--argc;
		} else
		if ( strcmp(argv[argc], "-fast") == 0 ) {
			videoflags = FastestFlags(videoflags, width, height, video_bpp);
		} else
		if ( strcmp(argv[argc], "-hw") == 0 ) {
			videoflags ^= SDL_HWSURFACE;
		} else
		if ( strcmp(argv[argc], "-flip") == 0 ) {
			videoflags ^= SDL_DOUBLEBUF;
		} else
		if ( strcmp(argv[argc], "-debugflip") == 0 ) {
			debug_flip ^= 1;
		} else
		if ( strcmp(argv[argc], "-fullscreen") == 0 ) {
			videoflags ^= SDL_FULLSCREEN;
		} else
		if ( isdigit(argv[argc][0]) ) {
			numsprites = atoi(argv[argc]);
		} else {
			fprintf(stderr, 
	"Usage: %s [-bpp N] [-hw] [-flip] [-fast] [-fullscreen] [numsprites]\n",
								argv[0]);
			quit(1);
		}
	}

	
	screen = SDL_SetVideoMode(width, height, video_bpp, videoflags);
	if ( ! screen ) {
		fprintf(stderr, "Couldn't set %dx%d video mode: %s\n",
					width, height, SDL_GetError());
		quit(2);
	}

	
	if ( LoadSprite("icon.bmp") < 0 ) {
		quit(1);
	}

	
	mem = (Uint8 *)malloc(4*sizeof(SDL_Rect)*numsprites);
	if ( mem == NULL ) {
		SDL_FreeSurface(sprite);
		fprintf(stderr, "Out of memory!\n");
		quit(2);
	}
	sprite_rects = (SDL_Rect *)mem;
	positions = sprite_rects;
	sprite_rects += numsprites;
	velocities = sprite_rects;
	sprite_rects += numsprites;
	sprite_w = sprite->w;
	sprite_h = sprite->h;
	srand(time(NULL));
	for ( i=0; i<numsprites; ++i ) {
		positions[i].x = rand()%(screen->w - sprite_w);
		positions[i].y = rand()%(screen->h - sprite_h);
		positions[i].w = sprite->w;
		positions[i].h = sprite->h;
		velocities[i].x = 0;
		velocities[i].y = 0;
		while ( ! velocities[i].x && ! velocities[i].y ) {
			velocities[i].x = (rand()%(MAX_SPEED*2+1))-MAX_SPEED;
			velocities[i].y = (rand()%(MAX_SPEED*2+1))-MAX_SPEED;
		}
	}
	background = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);

	
	printf("Screen is at %d bits per pixel\n",screen->format->BitsPerPixel);
	if ( (screen->flags & SDL_HWSURFACE) == SDL_HWSURFACE ) {
		printf("Screen is in video memory\n");
	} else {
		printf("Screen is in system memory\n");
	}
	if ( (screen->flags & SDL_DOUBLEBUF) == SDL_DOUBLEBUF ) {
		printf("Screen has double-buffering enabled\n");
	}
	if ( (sprite->flags & SDL_HWSURFACE) == SDL_HWSURFACE ) {
		printf("Sprite is in video memory\n");
	} else {
		printf("Sprite is in system memory\n");
	}
	
	{ SDL_Rect dst;
		dst.x = 0;
		dst.y = 0;
		dst.w = sprite->w;
		dst.h = sprite->h;
		SDL_BlitSurface(sprite, NULL, screen, &dst);
		SDL_FillRect(screen, &dst, background);
	}
	if ( (sprite->flags & SDL_HWACCEL) == SDL_HWACCEL ) {
		printf("Sprite blit uses hardware acceleration\n");
	}
	if ( (sprite->flags & SDL_RLEACCEL) == SDL_RLEACCEL ) {
		printf("Sprite blit uses RLE acceleration\n");
	}

	
	frames = 0;
	then = SDL_GetTicks();
	done = 0;
	sprites_visible = 0;
	while ( !done ) {
		
		++frames;
		while ( SDL_PollEvent(&event) ) {
			switch (event.type) {
				case SDL_MOUSEBUTTONDOWN:
					SDL_WarpMouse(screen->w/2, screen->h/2);
					break;
				case SDL_KEYDOWN:
					
				case SDL_QUIT:
					done = 1;
					break;
				default:
					break;
			}
		}
		MoveSprites(screen, background);
	}
	SDL_FreeSurface(sprite);
	free(mem);

	
	now = SDL_GetTicks();
	if ( now > then ) {
		printf("%2.2f frames per second\n",
					((double)frames*1000)/(now-then));
	}
	SDL_Quit();
	return(0);
}

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2012 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org

    Micah Dowty
    micahjd@users.sourceforge.net
*/
#include "SDL_config.h"

#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"

#include "SDL_pgvideo.h"
#include "SDL_pgevents_c.h"

#define PGVID_DRIVER_NAME "picogui"

static int PG_VideoInit(_THIS, SDL_PixelFormat *vformat);
static SDL_Rect **PG_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags);
static SDL_Surface *PG_SetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags);
static int PG_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors);
static void PG_VideoQuit(_THIS);

static int PG_AllocHWSurface(_THIS, SDL_Surface *surface);
static int PG_LockHWSurface(_THIS, SDL_Surface *surface);
static void PG_UnlockHWSurface(_THIS, SDL_Surface *surface);
static void PG_FreeHWSurface(_THIS, SDL_Surface *surface);

static void PG_UpdateRects(_THIS, int numrects, SDL_Rect *rects);

struct WMcursor {
  
  pghandle theme;
} ;

void PG_SetCaption(_THIS, const char *title, const char *icon);
WMcursor * PG_CreateWMCursor (_THIS,Uint8 * data, Uint8 * mask, 
			      int w, int h, int hot_x, int hot_y);
void PG_FreeWMCursor (_THIS, WMcursor * cursor);
void PG_WarpWMCursor(_THIS, Uint16 x, Uint16 y);
int PG_ShowWMCursor (_THIS, WMcursor * cursor);


static int PG_Available(void)
{
	return(1);
}

static void PG_DeleteDevice(SDL_VideoDevice *device)
{
	SDL_free(device->hidden);
	SDL_free(device);
}

static SDL_VideoDevice *PG_CreateDevice(int devindex)
{
	SDL_VideoDevice *device;

	
	device = (SDL_VideoDevice *)SDL_malloc(sizeof(SDL_VideoDevice));
	if ( device ) {
		SDL_memset(device, 0, (sizeof *device));
		device->hidden = (struct SDL_PrivateVideoData *)
				SDL_malloc((sizeof *device->hidden));
	}
	if ( (device == NULL) || (device->hidden == NULL) ) {
		SDL_OutOfMemory();
		if ( device ) {
			SDL_free(device);
		}
		return(0);
	}
	SDL_memset(device->hidden, 0, (sizeof *device->hidden));

	
	device->VideoInit = PG_VideoInit;
	device->ListModes = PG_ListModes;
	device->SetVideoMode = PG_SetVideoMode;
	device->CreateYUVOverlay = NULL;
	device->SetColors = PG_SetColors;
	device->UpdateRects = PG_UpdateRects;
	device->VideoQuit = PG_VideoQuit;
	device->AllocHWSurface = PG_AllocHWSurface;
	device->CheckHWBlit = NULL;
	device->FillHWRect = NULL;
	device->SetHWColorKey = NULL;
	device->SetHWAlpha = NULL;
	device->LockHWSurface = PG_LockHWSurface;
	device->UnlockHWSurface = PG_UnlockHWSurface;
	device->FlipHWSurface = NULL;
	device->FreeHWSurface = PG_FreeHWSurface;
	device->SetCaption = PG_SetCaption;
	device->SetIcon = NULL;
	device->IconifyWindow = NULL;
	device->GrabInput = NULL;

	device->PumpEvents = PG_PumpEvents;
	device->InitOSKeymap = PG_InitOSKeymap;

	device->ShowWMCursor = PG_ShowWMCursor;
	device->CreateWMCursor = PG_CreateWMCursor;
	device->FreeWMCursor = PG_FreeWMCursor;
	device->WarpWMCursor = PG_WarpWMCursor;

	device->free = PG_DeleteDevice;

	return device;
}

VideoBootStrap PG_bootstrap = {
	PGVID_DRIVER_NAME, "PicoGUI SDL driver",
	PG_Available, PG_CreateDevice
};


int PG_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
        pgInit(0,(char**)"");
	this->hidden->mi = *pgGetVideoMode();

	
	this->hidden->wApp = pgRegisterApp(PG_APP_NORMAL,"SDL",0);
	this->hidden->wCanvas = pgNewWidget(PG_WIDGET_CANVAS,0,0);
	pgSetWidget(PGDEFAULT,
		    PG_WP_SIDE, PG_S_ALL,
		    0);

	PG_InitEvents(this);

	
	this->info.current_w = this->hidden->mi.lxres;
	this->info.current_h = this->hidden->mi.lyres;

	vformat->BitsPerPixel = this->hidden->mi.bpp;
	vformat->BytesPerPixel = this->hidden->mi.bpp >> 3;
	if (this->hidden->mi.bpp & 7)
	  vformat->BytesPerPixel++;

	
	return(0);
}

SDL_Rect **PG_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags)
{
   	 return (SDL_Rect **) -1;
}

SDL_Surface *PG_SetVideoMode(_THIS, SDL_Surface *current,
				int width, int height, int bpp, Uint32 flags)
{
	if ( this->hidden->bitmap ) {
	  
	  if (current->pixels) {
	    shmdt(current->pixels);
	    current->pixels = NULL;
	  }
	  pgDelete(this->hidden->bitmap);
	}

	
	if ( ! SDL_ReallocFormat(current, bpp, 0, 0, 0, 0) ) {
		SDL_SetError("Couldn't allocate new pixel format for requested mode");
		return(NULL);
	}

	
	this->hidden->bitmap = pgCreateBitmap(width,height);
	this->hidden->shm = *pgMakeSHMBitmap(this->hidden->bitmap);
	current->pixels = shmat(shmget(this->hidden->shm.shm_key,
				       this->hidden->shm.shm_length,0),NULL,0);


	pgWriteCmd(this->hidden->wCanvas, PGCANVAS_NUKE, 0);

	pgWriteCmd(this->hidden->wCanvas, PGCANVAS_GROP, 5, PG_GROP_SETSRC,0,0,0,0);
	pgWriteCmd(this->hidden->wCanvas, PGCANVAS_GROPFLAGS, 1, PG_GROPF_INCREMENTAL);

	pgWriteCmd(this->hidden->wCanvas, PGCANVAS_GROP, 6, PG_GROP_BITMAP,
		   0,0,0,0,this->hidden->bitmap);
	pgWriteCmd(this->hidden->wCanvas, PGCANVAS_GROPFLAGS, 1, PG_GROPF_INCREMENTAL);

	pgWriteCmd(this->hidden->wCanvas, PGCANVAS_GROP, 6, PG_GROP_BITMAP,
		   0,0,this->hidden->shm.width,this->hidden->shm.height,this->hidden->bitmap);
     
	
	current->flags = 0;
	current->w = this->hidden->shm.width;
	current->h = this->hidden->shm.height;
	current->pitch = this->hidden->shm.pitch;

	
	current->format->BitsPerPixel = this->hidden->shm.bpp;
	current->format->BytesPerPixel = this->hidden->shm.bpp >> 3;
	if (this->hidden->shm.bpp & 7)
	  current->format->BytesPerPixel++;
	current->format->palette = NULL;
	current->format->Rmask = this->hidden->shm.red_mask;
	current->format->Gmask = this->hidden->shm.green_mask;
	current->format->Bmask = this->hidden->shm.blue_mask;
	current->format->Amask = this->hidden->shm.alpha_mask;
	current->format->Rshift = this->hidden->shm.red_shift;
	current->format->Gshift = this->hidden->shm.green_shift;
	current->format->Bshift = this->hidden->shm.blue_shift;
	current->format->Ashift = this->hidden->shm.alpha_shift;
	current->format->Rloss = 8 - this->hidden->shm.red_length;
	current->format->Gloss = 8 - this->hidden->shm.green_length;
	current->format->Bloss = 8 - this->hidden->shm.blue_length;
	current->format->Aloss = 8 - this->hidden->shm.alpha_length;

	
	pgUpdate();

	
	return(current);
}

static int PG_AllocHWSurface(_THIS, SDL_Surface *surface)
{
	return(-1);
}
static void PG_FreeHWSurface(_THIS, SDL_Surface *surface)
{
	return;
}

static int PG_LockHWSurface(_THIS, SDL_Surface *surface)
{
	return(0);
}

static void PG_UnlockHWSurface(_THIS, SDL_Surface *surface)
{
	return;
}

static void PG_UpdateRects(_THIS, int numrects, SDL_Rect *rects)
{
        int i;
	
	for (i = 0; i < numrects; i++) {
	        if (rects[i].w <= 0 || rects[i].h <= 0)
		  continue;

		pgWriteCmd(this->hidden->wCanvas, PGCANVAS_FINDGROP, 1, 0);
		pgWriteCmd(this->hidden->wCanvas, PGCANVAS_MOVEGROP, 4, 
			   rects[i].x, rects[i].y,
			   rects[i].w, rects[i].h);
		pgWriteCmd(this->hidden->wCanvas, PGCANVAS_FINDGROP, 1, 1);
		pgWriteCmd(this->hidden->wCanvas, PGCANVAS_MOVEGROP, 4, 
			   rects[i].x, rects[i].y,
			   rects[i].w, rects[i].h);
		
		
		pgWriteCmd(this->hidden->wCanvas, PGCANVAS_INCREMENTAL, 0);
		pgSubUpdate(this->hidden->wCanvas);
	}
}

int PG_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors)
{
	
	return(1);
}

void PG_VideoQuit(_THIS)
{
	if (this->screen->pixels != NULL)
	{
		shmdt(this->screen->pixels);
		this->screen->pixels = NULL;
		pgDelete(this->hidden->bitmap);
	}
	pgDelete(this->hidden->wCanvas);
	pgDelete(this->hidden->wApp);
}

void PG_SetCaption(_THIS, const char *title, const char *icon)
{
        if (title != NULL)
	  pgReplaceText(this->hidden->wApp, title);
	pgUpdate();
}


WMcursor * PG_CreateWMCursor (_THIS,Uint8 * data, Uint8 * mask, 
			      int w, int h, int hot_x, int hot_y)
{
        static WMcursor dummy;
        return &dummy;
}

void PG_FreeWMCursor (_THIS, WMcursor * cursor)
{
}

void PG_WarpWMCursor(_THIS, Uint16 x, Uint16 y)
{
}

int PG_ShowWMCursor (_THIS, WMcursor * cursor)
{
        return 1;
}

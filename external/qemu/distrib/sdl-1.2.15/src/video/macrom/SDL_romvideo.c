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
*/
#include "SDL_config.h"

#if defined(__APPLE__) && defined(__MACH__)
#include <Carbon/Carbon.h>
#if USE_QUICKTIME
#include <QuickTime/Movies.h>
#endif
#elif TARGET_API_MAC_CARBON && (UNIVERSAL_INTERFACES_VERSION > 0x0335)
#include <Carbon.h>
#if USE_QUICKTIME
#include <Movies.h>
#endif
#else
#include <Quickdraw.h>
#include <LowMem.h>
#include <Gestalt.h>
#include <Devices.h>
#include <DiskInit.h>
#include <QDOffscreen.h>
#endif

#include "SDL_video.h"
#include "SDL_syswm.h"
#include "../SDL_sysvideo.h"
#include "SDL_romvideo.h"
#include "../maccommon/SDL_macgl_c.h"
#include "../maccommon/SDL_macwm_c.h"
#include "../maccommon/SDL_macmouse_c.h"
#include "../maccommon/SDL_macevents_c.h"

static int ROM_VideoInit(_THIS, SDL_PixelFormat *vformat);
static SDL_Rect **ROM_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags);
static SDL_Surface *ROM_SetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags);
static int ROM_SetColors(_THIS, int firstcolor, int ncolors,
			 SDL_Color *colors);
static void ROM_VideoQuit(_THIS);

static int ROM_AllocHWSurface(_THIS, SDL_Surface *surface);
static int ROM_LockHWSurface(_THIS, SDL_Surface *surface);
static void ROM_UnlockHWSurface(_THIS, SDL_Surface *surface);
static void ROM_FreeHWSurface(_THIS, SDL_Surface *surface);

#if !TARGET_API_MAC_CARBON 
static RgnHandle	gSaveGrayRgn = nil;
static short		gSaveMenuBar = 0;
static Boolean		gSaveCSVis = true;

#if powerc
#define ONEWORDSTUB(p1) \
		{ 0x41FA, 0x0010, 0x209F, (p1), 0x41FA, \
		  0x0008, 0x2F10, 0x4E75, 0x0000, 0x0000, 0x0000 }

#define TWOWORDSTUB(p1,p2) \
		{ 0x41FA, 0x0012, 0x209F, (p1), (p2), 0x41FA, \
		  0x0008, 0x2F10, 0x4E75, 0x0000, 0x0000, 0x0000 }

#define THREEWORDSTUB(p1,p2,p3) \
		{ 0x41FA, 0x0014, 0x209F, (p1), (p2), (p3), 0x41FA, \
		  0x0008, 0x2F10, 0x4E75, 0x0000, 0x0000, 0x0000 }

static pascal Boolean SBIsControlStripVisible(void)
{
	static short procData[] = TWOWORDSTUB(0x7000, 0xAAF2);
	ProcInfoType procInfo = kD0DispatchedPascalStackBased
				| RESULT_SIZE(SIZE_CODE(sizeof(Boolean)))
            	| DISPATCHED_STACK_ROUTINE_SELECTOR_SIZE(kFourByteCode);
            				
	return((Boolean) CallUniversalProc((UniversalProcPtr) procData, procInfo, 0x00));
}

static pascal void SBShowHideControlStrip(Boolean showIt)
{
	static short procData[] = THREEWORDSTUB(0x303C, 0x0101, 0xAAF2);
	ProcInfoType procInfo = kD0DispatchedPascalStackBased
				| DISPATCHED_STACK_ROUTINE_SELECTOR_SIZE(kFourByteCode)
				| DISPATCHED_STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(Boolean)));

	CallUniversalProc((UniversalProcPtr) procData, procInfo, 0x01, showIt);
}
#endif 
#endif 


static int ROM_Available(void)
{
	return(1);
}

static void ROM_DeleteDevice(SDL_VideoDevice *device)
{
	SDL_free(device->hidden);
	SDL_free(device);
}

static SDL_VideoDevice *ROM_CreateDevice(int devindex)
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

	
	device->VideoInit = ROM_VideoInit;
	device->ListModes = ROM_ListModes;
	device->SetVideoMode = ROM_SetVideoMode;
	device->SetColors = ROM_SetColors;
	device->UpdateRects = NULL;
	device->VideoQuit = ROM_VideoQuit;
	device->AllocHWSurface = ROM_AllocHWSurface;
	device->CheckHWBlit = NULL;
	device->FillHWRect = NULL;
	device->SetHWColorKey = NULL;
	device->SetHWAlpha = NULL;
	device->LockHWSurface = ROM_LockHWSurface;
	device->UnlockHWSurface = ROM_UnlockHWSurface;
	device->FlipHWSurface = NULL;
	device->FreeHWSurface = ROM_FreeHWSurface;
#if SDL_MACCLASSIC_GAMMA_SUPPORT
	device->SetGammaRamp = Mac_SetGammaRamp;
	device->GetGammaRamp = Mac_GetGammaRamp;
#endif
#if SDL_VIDEO_OPENGL
	device->GL_MakeCurrent = Mac_GL_MakeCurrent;
	device->GL_SwapBuffers = Mac_GL_SwapBuffers;
	device->GL_LoadLibrary = Mac_GL_LoadLibrary;
	device->GL_GetProcAddress = Mac_GL_GetProcAddress;
#endif	
	device->SetCaption = Mac_SetCaption;
	device->SetIcon = NULL;
	device->IconifyWindow = NULL;
	device->GrabInput = NULL;
	device->GetWMInfo = NULL;
	device->FreeWMCursor = Mac_FreeWMCursor;
	device->CreateWMCursor = Mac_CreateWMCursor;
	device->ShowWMCursor = Mac_ShowWMCursor;
	device->WarpWMCursor = Mac_WarpWMCursor;
	device->InitOSKeymap = Mac_InitOSKeymap;
	device->PumpEvents = Mac_PumpEvents;

	device->free = ROM_DeleteDevice;

	return device;
}

VideoBootStrap TOOLBOX_bootstrap = {
	"toolbox", "MacOS ROM Toolbox",
	ROM_Available, ROM_CreateDevice
};


static int ROM_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
	long info;
	
	
	Gestalt(gestaltQuickdrawVersion, &info);
	if ( info == gestaltOriginalQD ) {
		SDL_SetError("Color Quickdraw not available");
		return(-1);
	}

	
	Mac_InitEvents(this);

	
	SDL_Display = GetMainDevice();

	
	this->info.current_w = (**SDL_Display).gdRect.right;
	this->info.current_h = (**SDL_Display).gdRect.bottom;

	
	vformat->BitsPerPixel = (**(**SDL_Display).gdPMap).pixelSize;
	switch (vformat->BitsPerPixel) {
		case 16:	
			vformat->Rmask = 0x00007c00;
			vformat->Gmask = 0x000003e0;
			vformat->Bmask = 0x0000001f;
			break;
		default:
			break;
	}

	
	SDL_CTab = (CTabHandle)NewHandle(sizeof(ColorSpec)*256 + 8);
	if ( SDL_CTab == nil ) {
		SDL_OutOfMemory();
		return(-1);
	}
	(**SDL_CTab).ctSeed = GetCTSeed();
	(**SDL_CTab).ctFlags = 0;
	(**SDL_CTab).ctSize = 255;
	CTabChanged(SDL_CTab);
	SDL_CPal = NewPalette(256, SDL_CTab, pmExplicit+pmTolerant, 0);

	
	SDL_modelist = (SDL_Rect **)SDL_malloc((1+1)*sizeof(SDL_Rect *));
	if ( SDL_modelist ) {
		SDL_modelist[0] = (SDL_Rect *)SDL_malloc(sizeof(SDL_Rect));
		if ( SDL_modelist[0] ) {
			SDL_modelist[0]->x = 0;
			SDL_modelist[0]->y = 0;
			SDL_modelist[0]->w = (**SDL_Display).gdRect.right;
			SDL_modelist[0]->h = (**SDL_Display).gdRect.bottom;
		}
		SDL_modelist[1] = NULL;
	}

	
	this->info.wm_available = 1;

	
	return(0);
}

static SDL_Rect **ROM_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags)
{
	if ( this->screen->format->BitsPerPixel == format->BitsPerPixel ) {
		if ( (flags & SDL_FULLSCREEN) == SDL_FULLSCREEN ) {
			return(SDL_modelist);
		} else {
			return((SDL_Rect **)-1);
		}
	} else {
		return((SDL_Rect **)0);
	}
}

static void ROM_HideMenuBar(_THIS)
{
#if !TARGET_API_MAC_CARBON 
	RgnHandle		drawRgn = nil;
	RgnHandle		tempRgn = nil;
	RgnHandle		grayRgn = nil;
	WindowPtr		window = nil;
	GDHandle		gd = nil;
	GrafPtr			savePort;
	long			response;
	short			height;
	EventRecord		theEvent;

	height = GetMBarHeight();
	
	if ( height > 0 ) {
		tempRgn = NewRgn();
		drawRgn = NewRgn();
		gSaveGrayRgn = NewRgn();
		if ( ! tempRgn || ! drawRgn || ! gSaveGrayRgn ) {
			goto CLEANUP;
		}
		grayRgn = GetGrayRgn(); 
	
		GetPort(&savePort);

		CopyRgn(grayRgn, tempRgn);
		if (!Gestalt(gestaltControlStripAttr, &response) && 
			(response & (1L << gestaltControlStripExists))) {
			gSaveCSVis = SBIsControlStripVisible();
			if (gSaveCSVis)
				SBShowHideControlStrip(false);
		}
		DiffRgn(grayRgn, tempRgn, drawRgn);

		
		CopyRgn(grayRgn, gSaveGrayRgn);

		
		gSaveMenuBar = height;
		LMSetMBarHeight(0);
		
		for(gd = GetDeviceList(); gd; gd = GetNextDevice(gd)) 
			{
			if (!TestDeviceAttribute(gd, screenDevice)) continue;
			if (!TestDeviceAttribute(gd, screenActive)) continue;

			RectRgn(tempRgn, &(*gd)->gdRect);	
			DiffRgn(tempRgn, grayRgn, tempRgn); 
			UnionRgn(tempRgn, drawRgn, drawRgn);
			}
			
		
		UnionRgn(drawRgn, grayRgn, grayRgn);

		
		window = (FrontWindow()) ? FrontWindow() : (WindowPtr) -1L;
		PaintBehind(window, drawRgn);
		CalcVisBehind(window, drawRgn);

		SetPort(savePort);
		
		
		EventAvail(0, &theEvent);
		EventAvail(0, &theEvent);
		EventAvail(0, &theEvent);
		EventAvail(0, &theEvent);
		}

CLEANUP:

	if (tempRgn) DisposeRgn(tempRgn);
	if (drawRgn) DisposeRgn(drawRgn);
#endif 
}
	
static void ROM_ShowMenuBar(_THIS)
{
#if !TARGET_API_MAC_CARBON 
	RgnHandle		drawRgn = nil;
	RgnHandle		menuRgn = nil;
	RgnHandle		tempRgn = nil;
	RgnHandle		grayRgn = nil;
	WindowPtr		window = nil;
	GrafPtr			wMgrPort;
	GrafPtr			savePort;
	Rect			menuRect;
	long			response;
	short			height;
	EventRecord		theEvent;
	RGBColor		saveRGB;
	RGBColor		blackRGB = { 0, 0, 0 };

	height = GetMBarHeight();
	
	if ((height <= 0) && (gSaveMenuBar > 0)) {
		drawRgn = NewRgn();
		menuRgn = NewRgn();
		tempRgn = NewRgn();
		if ( ! tempRgn || ! drawRgn || ! gSaveGrayRgn ) {
			goto CLEANUP;
		}
		grayRgn = GetGrayRgn(); 
	
		GetPort(&savePort);
		GetWMgrPort(&wMgrPort);

		
		LMSetMBarHeight(gSaveMenuBar);

		if (gSaveGrayRgn)
			{
			menuRect = (*GetMainDevice())->gdRect;
			menuRect.bottom = menuRect.top + gSaveMenuBar;
			RectRgn(menuRgn, &menuRect);

			DiffRgn(grayRgn, gSaveGrayRgn, drawRgn); 	
			DiffRgn(drawRgn, menuRgn, drawRgn);			
			
			
			SetPort(wMgrPort);
			GetClip(tempRgn);
			SetClip(drawRgn);
			GetForeColor(&saveRGB);
			RGBForeColor(&blackRGB);
			PaintRgn(drawRgn);
			RGBForeColor(&saveRGB);
			SetClip(tempRgn);
			SetPort(savePort);
			
			UnionRgn(drawRgn, menuRgn, drawRgn);		

			
			CopyRgn(gSaveGrayRgn, grayRgn);
			DisposeRgn(gSaveGrayRgn);
			gSaveGrayRgn = nil;
			}

		
		window = (FrontWindow()) ? FrontWindow() : (WindowPtr) -1L;
		PaintBehind(window, drawRgn);
		CalcVisBehind(window, drawRgn);
		DrawMenuBar();

		SetPort(savePort);
		gSaveMenuBar = 0;

		
		if (!Gestalt(gestaltControlStripAttr, &response) && 
				(response & (1L << gestaltControlStripExists)))
			{
			if (gSaveCSVis && !SBIsControlStripVisible())
				SBShowHideControlStrip(true);
			gSaveCSVis = true;
			}

		
		EventAvail(0, &theEvent);
		EventAvail(0, &theEvent);
		EventAvail(0, &theEvent);
		EventAvail(0, &theEvent);
		}

CLEANUP:

	if (drawRgn) DisposeRgn(drawRgn);
	if (menuRgn) DisposeRgn(menuRgn);
	if (tempRgn) DisposeRgn(tempRgn);
#endif 
}

static void ROM_DirectUpdate(_THIS, int numrects, SDL_Rect *rects);
static void ROM_WindowUpdate(_THIS, int numrects, SDL_Rect *rects);

static void ROM_UnsetVideoMode(_THIS, SDL_Surface *current)
{
	
	if ( SDL_Window != nil ) {
		GWorldPtr memworld;
		
		
		Mac_GL_Quit(this);

		memworld = (GWorldPtr)GetWRefCon(SDL_Window);
		if ( memworld != nil ) {
			UnlockPixels(GetGWorldPixMap(memworld));
			DisposeGWorld(memworld);
		}
		if ( (current->flags & SDL_FULLSCREEN) == SDL_FULLSCREEN ) {
#if USE_QUICKTIME
			EndFullScreen(fullscreen_ctx, nil);
			SDL_Window = nil;
#else
			ROM_ShowMenuBar(this);
#endif
		}
	}
	current->pixels = NULL;
	current->flags &= ~(SDL_HWSURFACE|SDL_FULLSCREEN);
}

static SDL_Surface *ROM_SetVideoMode(_THIS, SDL_Surface *current,
				int width, int height, int bpp, Uint32 flags)
{
	Rect wrect, orect;
#if TARGET_API_MAC_CARBON
	Rect tmprect;
#endif

	
	ROM_UnsetVideoMode(this, current);

	
	current->flags = 0;		
	current->w = width;
	current->h = height;
	SetRect(&wrect, 0, 0, width, height);
	if ( SDL_Window ) {
		
#if TARGET_API_MAC_CARBON
		orect = *GetWindowPortBounds(SDL_Window, &tmprect);
#else
		orect = SDL_Window->portRect;
#endif
		OffsetRect(&wrect, orect.left, orect.top);
	} else {
		
		OffsetRect(&wrect,
		(SDL_modelist[0]->w-width)/2, (SDL_modelist[0]->h-height)/2);
	}

#if defined(__MACOSX__) && !USE_QUICKTIME
	
	flags &= ~SDL_FULLSCREEN;
#endif
	if ( (flags & SDL_FULLSCREEN) == SDL_FULLSCREEN ) {
		
		current->flags |= SDL_HWSURFACE|SDL_FULLSCREEN;
		if ( SDL_Window ) {
			DisposeWindow(SDL_Window);
		}
#if USE_QUICKTIME
		BeginFullScreen(&fullscreen_ctx, nil, 0,0, &SDL_Window, nil, 0);
#else
		SDL_Window = NewCWindow(nil, &wrect, "\p", true, plainDBox,
						(WindowPtr)-1, false, 0);
		ROM_HideMenuBar(this);
#endif
		current->pitch = (**(**SDL_Display).gdPMap).rowBytes & 0x3FFF;
		current->pixels = (**(**SDL_Display).gdPMap).baseAddr;
		this->UpdateRects = ROM_DirectUpdate;
	} else {
		GWorldPtr memworld;
		PixMapHandle pixmap;
		int style;

		style = noGrowDocProc;
		if ( flags & SDL_NOFRAME ) {
			style = plainDBox;
			current->flags |= SDL_NOFRAME;
		} else
		if ( flags & SDL_RESIZABLE ) {
			style = zoomDocProc;
			current->flags |= SDL_RESIZABLE;
		}
		if ( SDL_Window && (style == current_style) ) {
			
			if ( ((orect.right-orect.left) != width) ||
			     ((orect.bottom-orect.top) != height) ) {
				SizeWindow(SDL_Window, width, height, false);
			}
		} else {
			
			if ( SDL_Window ) {
				DisposeWindow(SDL_Window);
			}
			SDL_Window = NewCWindow(nil, &wrect, "\p", true,
			                        style, (WindowPtr)-1, true, 0);

			
			{ char *title;
				SDL_WM_GetCaption(&title, NULL);
				if ( title ) {
					Mac_SetCaption(this, title, NULL);
				}
			}
		}
		current_style = style;
		SetPalette(SDL_Window, SDL_CPal, false);
		ActivatePalette(SDL_Window);
		if ( NewGWorld(&memworld, 0,
#if TARGET_API_MAC_CARBON
			       GetWindowPortBounds(SDL_Window, &tmprect),
#else
			       &SDL_Window->portRect,
#endif
			       SDL_CTab, nil, 0) != noErr ) {
			SDL_SetError("NewGWorld() failed");
			return(NULL);
		}
		SetWRefCon(SDL_Window, (long)memworld);
		pixmap = GetGWorldPixMap(memworld);
		LockPixels(pixmap);
		current->pitch = (**pixmap).rowBytes & 0x3FFF;
		current->pixels = GetPixBaseAddr(pixmap);
		this->UpdateRects = ROM_WindowUpdate;
	}
	SetPortWindowPort(SDL_Window);
	SelectWindow(SDL_Window);

	
	if ( flags & SDL_OPENGL ) {
		if ( Mac_GL_Init(this) == 0 ) {
			current->flags |= SDL_OPENGL;
		} else {
			current = NULL;
		}
	}
	
	if ( (flags & SDL_HWPALETTE) && (flags & SDL_FULLSCREEN) )
	   current->flags |= SDL_HWPALETTE;
	   
	
	return(current);
}

static int ROM_AllocHWSurface(_THIS, SDL_Surface *surface)
{
	return(-1);
}
static void ROM_FreeHWSurface(_THIS, SDL_Surface *surface)
{
	return;
}
static int ROM_LockHWSurface(_THIS, SDL_Surface *surface)
{
	return(0);
}
static void ROM_UnlockHWSurface(_THIS, SDL_Surface *surface)
{
	return;
}

static void ROM_DirectUpdate(_THIS, int numrects, SDL_Rect *rects)
{
	
	return;
}

static void ROM_WindowUpdate(_THIS, int numrects, SDL_Rect *rects)
{
	GWorldPtr memworld;
	GrafPtr saveport;
	CGrafPtr thePort;
	const BitMap *memBits;
	const BitMap *winBits;
	int i;
	Rect update;
	
	
	GetPort(&saveport);
	SetPortWindowPort(SDL_Window);
	thePort = GetWindowPort(SDL_Window);
	memworld = (GWorldPtr)GetWRefCon(SDL_Window);
#if TARGET_API_MAC_CARBON && ACCESSOR_CALLS_ARE_FUNCTIONS
	memBits = GetPortBitMapForCopyBits((CGrafPtr) memworld);
#else
	memBits = &((GrafPtr)memworld)->portBits;
#endif
#if TARGET_API_MAC_CARBON && ACCESSOR_CALLS_ARE_FUNCTIONS
	winBits = GetPortBitMapForCopyBits(thePort);
#else
	winBits = &SDL_Window->portBits;
#endif
	for ( i=0; i<numrects; ++i ) {
		update.left = rects[i].x;
		update.right = rects[i].x+rects[i].w;
		update.top = rects[i].y;
		update.bottom = rects[i].y+rects[i].h;
		CopyBits(memBits, winBits,
			 &update, &update, srcCopy, nil);
	}
#if TARGET_API_MAC_CARBON
	if ( QDIsPortBuffered(thePort) ) {
		QDFlushPortBuffer(thePort, NULL);
	}
#endif
	SetPort(saveport);
}

static int ROM_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors)
{
	CTabHandle cTab;
	int i;

	
	if ( (this->screen->flags & SDL_FULLSCREEN) == SDL_FULLSCREEN ) {
		cTab = (**(**SDL_Display).gdPMap).pmTable;
	} else {
		cTab = SDL_CTab;
	}

	
	if ( (firstcolor+ncolors) > ((**cTab).ctSize+1) ) {
		return(0);
	}
	
	
	for ( i=0; i< ncolors; ++i ) {
	        int j = firstcolor + i;
		(**cTab).ctTable[j].value = j;
		(**cTab).ctTable[j].rgb.red = colors[i].r << 8 | colors[i].r;
		(**cTab).ctTable[j].rgb.green = colors[i].g << 8 | colors[i].g;
		(**cTab).ctTable[j].rgb.blue = colors[i].b << 8 | colors[i].b;
	}

#if 0
	if ( (this->screen->flags & SDL_FULLSCREEN) == SDL_FULLSCREEN )
#endif
	{
		GDevice **odisplay;
		odisplay = GetGDevice();
		SetGDevice(SDL_Display);
		SetEntries(0, (**cTab).ctSize, (ColorSpec *)&(**cTab).ctTable);
		SetGDevice(odisplay);
	}
	return(1);
}

void ROM_VideoQuit(_THIS)
{
	int i;

	
	ROM_UnsetVideoMode(this, this->screen);
	if ( SDL_Window ) {
		DisposeWindow(SDL_Window);
		SDL_Window = nil;
	}

	
	if ( SDL_CTab != nil ) {
		DisposeHandle((Handle)SDL_CTab);
		SDL_CTab = nil;
	}
	if ( SDL_CPal != nil ) {
		DisposePalette(SDL_CPal);
		SDL_CPal = nil;
	}
	RestoreDeviceClut(GetMainDevice());

#if SDL_MACCLASSIC_GAMMA_SUPPORT
	Mac_QuitGamma(this);
#endif

	
	if ( SDL_modelist != NULL ) {
		for ( i=0; SDL_modelist[i]; ++i ) {
			SDL_free(SDL_modelist[i]);
		}
		SDL_free(SDL_modelist);
		SDL_modelist = NULL;
	}
}


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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifndef CDS_FULLSCREEN
#define CDS_FULLSCREEN	4
#endif

#include "SDL_syswm.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_sysevents.h"
#include "../../events/SDL_events_c.h"
#include "SDL_gapidibvideo.h"
#include "SDL_dibvideo.h"
#include "../wincommon/SDL_syswm_c.h"
#include "../wincommon/SDL_sysmouse_c.h"
#include "SDL_dibevents_c.h"
#include "../wincommon/SDL_wingl_c.h"

#ifdef _WIN32_WCE

#ifndef DM_DISPLAYORIENTATION
#define DM_DISPLAYORIENTATION 0x00800000L
#endif
#ifndef DM_DISPLAYQUERYORIENTATION 
#define DM_DISPLAYQUERYORIENTATION 0x01000000L
#endif
#ifndef DMDO_0
#define DMDO_0      0
#endif
#ifndef DMDO_90
#define DMDO_90     1
#endif
#ifndef DMDO_180
#define DMDO_180    2
#endif
#ifndef DMDO_270
#define DMDO_270    4
#endif

#define NO_GETDIBITS
#define NO_GAMMA_SUPPORT
  #if _WIN32_WCE < 420
    #define NO_CHANGEDISPLAYSETTINGS
  #else
    #define ChangeDisplaySettings(lpDevMode, dwFlags) ChangeDisplaySettingsEx(NULL, (lpDevMode), 0, (dwFlags), 0)
  #endif
#endif
#ifndef WS_MAXIMIZE
#define WS_MAXIMIZE	0
#endif
#ifndef WS_THICKFRAME
#define WS_THICKFRAME	0
#endif
#ifndef SWP_NOCOPYBITS
#define SWP_NOCOPYBITS	0
#endif
#ifndef PC_NOCOLLAPSE
#define PC_NOCOLLAPSE	0
#endif

#ifdef _WIN32_WCE
extern HINSTANCE aygshell;
#endif

static int DIB_VideoInit(_THIS, SDL_PixelFormat *vformat);
static SDL_Rect **DIB_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags);
SDL_Surface *DIB_SetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags);
static int DIB_SetColors(_THIS, int firstcolor, int ncolors,
			 SDL_Color *colors);
static void DIB_CheckGamma(_THIS);
void DIB_SwapGamma(_THIS);
void DIB_QuitGamma(_THIS);
int DIB_SetGammaRamp(_THIS, Uint16 *ramp);
int DIB_GetGammaRamp(_THIS, Uint16 *ramp);
static void DIB_VideoQuit(_THIS);

static int DIB_AllocHWSurface(_THIS, SDL_Surface *surface);
static int DIB_LockHWSurface(_THIS, SDL_Surface *surface);
static void DIB_UnlockHWSurface(_THIS, SDL_Surface *surface);
static void DIB_FreeHWSurface(_THIS, SDL_Surface *surface);

static void DIB_GrabStaticColors(HWND window);
static void DIB_ReleaseStaticColors(HWND window);
static void DIB_Activate(_THIS, BOOL active, BOOL minimized);
static void DIB_RealizePalette(_THIS);
static void DIB_PaletteChanged(_THIS, HWND window);
static void DIB_WinPAINT(_THIS, HDC hdc);

static void DIB_GetWinPos(_THIS, int* px, int *py);
static void DIB_SetWinPos(_THIS, int  x, int  y);
static int  DIB_IsWinVisible(_THIS, int recenter);
static int  DIB_GetMonitorDPI(_THIS, int* xDpi, int *yDpi);
static int  DIB_GetMonitorRect(_THIS, SDL_Rect*  rect);

static int DIB_SussScreenDepth();


static int DIB_Available(void)
{
	return(1);
}

static void DIB_DeleteDevice(SDL_VideoDevice *device)
{
	if ( device ) {
		if ( device->hidden ) {
			if ( device->hidden->dibInfo ) {
				SDL_free( device->hidden->dibInfo );
			}
			SDL_free(device->hidden);
		}
		if ( device->gl_data ) {
			SDL_free(device->gl_data);
		}
		SDL_free(device);
	}
}

static SDL_VideoDevice *DIB_CreateDevice(int devindex)
{
	SDL_VideoDevice *device;

	
	device = (SDL_VideoDevice *)SDL_malloc(sizeof(SDL_VideoDevice));
	if ( device ) {
		SDL_memset(device, 0, (sizeof *device));
		device->hidden = (struct SDL_PrivateVideoData *)
				SDL_malloc((sizeof *device->hidden));
		if(device->hidden){
			SDL_memset(device->hidden, 0, (sizeof *device->hidden));
			device->hidden->dibInfo = (DibInfo *)SDL_malloc((sizeof(DibInfo)));
			if(device->hidden->dibInfo == NULL)
			{
				SDL_free(device->hidden);
				device->hidden = NULL;
			}
		}
		
		device->gl_data = (struct SDL_PrivateGLData *)
				SDL_malloc((sizeof *device->gl_data));
	}
	if ( (device == NULL) || (device->hidden == NULL) ||
		                 (device->gl_data == NULL) ) {
		SDL_OutOfMemory();
		DIB_DeleteDevice(device);
		return(NULL);
	}
	SDL_memset(device->hidden->dibInfo, 0, (sizeof *device->hidden->dibInfo));
	SDL_memset(device->gl_data, 0, (sizeof *device->gl_data));

	
	device->VideoInit = DIB_VideoInit;
	device->ListModes = DIB_ListModes;
	device->SetVideoMode = DIB_SetVideoMode;
	device->UpdateMouse = WIN_UpdateMouse;
	device->SetColors = DIB_SetColors;
	device->UpdateRects = NULL;
	device->VideoQuit = DIB_VideoQuit;
	device->AllocHWSurface = DIB_AllocHWSurface;
	device->CheckHWBlit = NULL;
	device->FillHWRect = NULL;
	device->SetHWColorKey = NULL;
	device->SetHWAlpha = NULL;
	device->LockHWSurface = DIB_LockHWSurface;
	device->UnlockHWSurface = DIB_UnlockHWSurface;
	device->FlipHWSurface = NULL;
	device->FreeHWSurface = DIB_FreeHWSurface;
	device->SetGammaRamp = DIB_SetGammaRamp;
	device->GetGammaRamp = DIB_GetGammaRamp;
#if SDL_VIDEO_OPENGL
	device->GL_LoadLibrary = WIN_GL_LoadLibrary;
	device->GL_GetProcAddress = WIN_GL_GetProcAddress;
	device->GL_GetAttribute = WIN_GL_GetAttribute;
	device->GL_MakeCurrent = WIN_GL_MakeCurrent;
	device->GL_SwapBuffers = WIN_GL_SwapBuffers;
#endif
	device->SetCaption = WIN_SetWMCaption;
	device->SetIcon = WIN_SetWMIcon;
	device->IconifyWindow = WIN_IconifyWindow;
	device->GrabInput = WIN_GrabInput;
	device->GetWMInfo = WIN_GetWMInfo;
	device->FreeWMCursor = WIN_FreeWMCursor;
	device->CreateWMCursor = WIN_CreateWMCursor;
	device->ShowWMCursor = WIN_ShowWMCursor;
	device->WarpWMCursor = WIN_WarpWMCursor;
	device->CheckMouseMode = WIN_CheckMouseMode;
	device->InitOSKeymap = DIB_InitOSKeymap;
	device->PumpEvents = DIB_PumpEvents;

	device->GetWindowPos = DIB_GetWinPos;
	device->SetWindowPos = DIB_SetWinPos;
	device->IsWindowVisible = DIB_IsWinVisible;
	device->GetMonitorDPI = DIB_GetMonitorDPI;
	device->GetMonitorRect = DIB_GetMonitorRect;

	
	WIN_Activate = DIB_Activate;
	WIN_RealizePalette = DIB_RealizePalette;
	WIN_PaletteChanged = DIB_PaletteChanged;
	WIN_WinPAINT = DIB_WinPAINT;
	HandleMessage = DIB_HandleMessage;

	device->free = DIB_DeleteDevice;

	
	return device;
}

VideoBootStrap WINDIB_bootstrap = {
	"windib", "Win95/98/NT/2000/CE GDI",
	DIB_Available, DIB_CreateDevice
};

static int cmpmodes(const void *va, const void *vb)
{
    SDL_Rect *a = *(SDL_Rect **)va;
    SDL_Rect *b = *(SDL_Rect **)vb;
    if ( a->w == b->w )
        return b->h - a->h;
    else
        return b->w - a->w;
}

static int DIB_AddMode(_THIS, int bpp, int w, int h)
{
	SDL_Rect *mode;
	int i, index;
	int next_mode;

	
	if ( bpp < 8 || bpp > 32 ) {  
		return(0);
	}
	index = ((bpp+7)/8)-1;
	for ( i=0; i<SDL_nummodes[index]; ++i ) {
		mode = SDL_modelist[index][i];
		if ( (mode->w == w) && (mode->h == h) ) {
			return(0);
		}
	}

	
	mode = (SDL_Rect *)SDL_malloc(sizeof *mode);
	if ( mode == NULL ) {
		SDL_OutOfMemory();
		return(-1);
	}
	mode->x = 0;
	mode->y = 0;
	mode->w = w;
	mode->h = h;

	
	next_mode = SDL_nummodes[index];
	SDL_modelist[index] = (SDL_Rect **)
	       SDL_realloc(SDL_modelist[index], (1+next_mode+1)*sizeof(SDL_Rect *));
	if ( SDL_modelist[index] == NULL ) {
		SDL_OutOfMemory();
		SDL_nummodes[index] = 0;
		SDL_free(mode);
		return(-1);
	}
	SDL_modelist[index][next_mode] = mode;
	SDL_modelist[index][next_mode+1] = NULL;
	SDL_nummodes[index]++;

	return(0);
}

static void DIB_CreatePalette(_THIS, int bpp)
{

	LOGPALETTE *palette;
	HDC hdc;
	int ncolors;

	ncolors = (1 << bpp);
	palette = (LOGPALETTE *)SDL_malloc(sizeof(*palette)+
				ncolors*sizeof(PALETTEENTRY));
	palette->palVersion = 0x300;
	palette->palNumEntries = ncolors;
	hdc = GetDC(SDL_Window);
	GetSystemPaletteEntries(hdc, 0, ncolors, palette->palPalEntry);
	ReleaseDC(SDL_Window, hdc);
	screen_pal = CreatePalette(palette);
	screen_logpal = palette;
}

int DIB_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
	const char *env = NULL;
#ifndef NO_CHANGEDISPLAYSETTINGS
	int i;
	DEVMODE settings;
#endif

	
	if ( DIB_CreateWindow(this) < 0 ) {
		return(-1);
	}

#if !SDL_AUDIO_DISABLED
	DX5_SoundFocus(SDL_Window);
#endif

	
	vformat->BitsPerPixel = DIB_SussScreenDepth();
	switch (vformat->BitsPerPixel) {
		case 15:
			vformat->Rmask = 0x00007c00;
			vformat->Gmask = 0x000003e0;
			vformat->Bmask = 0x0000001f;
			vformat->BitsPerPixel = 16;
			break;
		case 16:
			vformat->Rmask = 0x0000f800;
			vformat->Gmask = 0x000007e0;
			vformat->Bmask = 0x0000001f;
			break;
		case 24:
		case 32:
			
			vformat->Rmask = 0x00ff0000;
			vformat->Gmask = 0x0000ff00;
			vformat->Bmask = 0x000000ff;
			break;
		default:
			break;
	}

	
	DIB_CheckGamma(this);

#ifndef NO_CHANGEDISPLAYSETTINGS

	settings.dmSize = sizeof(DEVMODE);
	settings.dmDriverExtra = 0;
#ifdef _WIN32_WCE
	settings.dmFields = DM_DISPLAYQUERYORIENTATION;
	this->hidden->supportRotation = ChangeDisplaySettingsEx(NULL, &settings, NULL, CDS_TEST, NULL) == DISP_CHANGE_SUCCESSFUL;
#endif
	
	SDL_desktop_mode.dmSize = sizeof(SDL_desktop_mode);
	SDL_desktop_mode.dmDriverExtra = 0;
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &SDL_desktop_mode);
	this->info.current_w = SDL_desktop_mode.dmPelsWidth;
	this->info.current_h = SDL_desktop_mode.dmPelsHeight;

	
	for ( i=0; EnumDisplaySettings(NULL, i, &settings); ++i ) {
		DIB_AddMode(this, settings.dmBitsPerPel,
			settings.dmPelsWidth, settings.dmPelsHeight);
#ifdef _WIN32_WCE		
		if( this->hidden->supportRotation )
			DIB_AddMode(this, settings.dmBitsPerPel,
				settings.dmPelsHeight, settings.dmPelsWidth);
#endif
	}
	
	for ( i=0; i<NUM_MODELISTS; ++i ) {
		if ( SDL_nummodes[i] > 0 ) {
			SDL_qsort(SDL_modelist[i], SDL_nummodes[i], sizeof *SDL_modelist[i], cmpmodes);
		}
	}
#else
	
	
	
	
	

	DIB_AddMode(this, vformat->BitsPerPixel,
			GetDeviceCaps(GetDC(NULL), HORZRES), 
			GetDeviceCaps(GetDC(NULL), VERTRES));

#endif 

	
	if ( vformat->BitsPerPixel <= 8 ) {
		DIB_CreatePalette(this, vformat->BitsPerPixel);
	}

	
	this->info.wm_available = 1;

#ifdef _WIN32_WCE
	this->hidden->origRotation = -1;
#endif

	
	env = SDL_getenv("SDL_VIDEO_ALLOW_SCREENSAVER");
	if ( env ) {
		allow_screensaver = SDL_atoi(env);
	} else {
#ifdef SDL_VIDEO_DISABLE_SCREENSAVER
		allow_screensaver = 0;
#else
		allow_screensaver = 1;
#endif
	}

	
	return(0);
}

SDL_Rect **DIB_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags)
{
	if ( (flags & SDL_FULLSCREEN) == SDL_FULLSCREEN ) {
		return(SDL_modelist[((format->BitsPerPixel+7)/8)-1]);
	} else {
		return((SDL_Rect **)-1);
	}
}


static int DIB_SussScreenDepth()
{
#ifdef NO_GETDIBITS
	int depth;
	HDC hdc;

	hdc = GetDC(SDL_Window);
	depth = GetDeviceCaps(hdc, PLANES) * GetDeviceCaps(hdc, BITSPIXEL);
	ReleaseDC(SDL_Window, hdc);
	return(depth);
#else
    int depth;
    int dib_size;
    LPBITMAPINFOHEADER dib_hdr;
    HDC hdc;
    HBITMAP hbm;

    dib_size = sizeof(BITMAPINFOHEADER) + 256 * sizeof (RGBQUAD);
    dib_hdr = (LPBITMAPINFOHEADER) SDL_malloc(dib_size);
    SDL_memset(dib_hdr, 0, dib_size);
    dib_hdr->biSize = sizeof(BITMAPINFOHEADER);
    
    hdc = GetDC(NULL);
    hbm = CreateCompatibleBitmap( hdc, 1, 1 );

    GetDIBits(hdc, hbm, 0, 1, NULL, (LPBITMAPINFO) dib_hdr, DIB_RGB_COLORS);
    GetDIBits(hdc, hbm, 0, 1, NULL, (LPBITMAPINFO) dib_hdr, DIB_RGB_COLORS);
    DeleteObject(hbm);
    ReleaseDC(NULL, hdc);

    depth = 0;
    switch( dib_hdr->biBitCount )
    {
    case 8:     depth = 8; break;
    case 24:    depth = 24; break;
    case 32:    depth = 32; break;
    case 16:
        if( dib_hdr->biCompression == BI_BITFIELDS ) {
            
            switch( ((DWORD*)((char*)dib_hdr + dib_hdr->biSize))[0] ) {
                case 0xf800: depth = 16; break;   
                case 0x7c00: depth = 15; break;   
            }
        }
    }
    SDL_free(dib_hdr);
    return depth;
#endif 
}


static void DIB_NormalUpdate(_THIS, int numrects, SDL_Rect *rects);

static void DIB_ResizeWindow(_THIS, int width, int height, int prev_width, int prev_height, Uint32 flags)
{
	RECT bounds;
	int x, y;

#ifndef _WIN32_WCE
	
	if ( !SDL_windowid && !IsZoomed(SDL_Window) ) {
#else
	if ( !SDL_windowid ) {
#endif
		HWND top;
		UINT swp_flags;
		const char *window = NULL;
		const char *center = NULL;

		if ( width != prev_width || height != prev_height ) {
			window = SDL_getenv("SDL_VIDEO_WINDOW_POS");
			center = SDL_getenv("SDL_VIDEO_CENTERED");
			if ( window ) {
				if ( SDL_sscanf(window, "%d,%d", &x, &y) == 2 ) {
					SDL_windowX = x;
					SDL_windowY = y;
				}
				if ( SDL_strcmp(window, "center") == 0 ) {
					center = window;
				}
			}
		}
		swp_flags = (SWP_NOCOPYBITS | SWP_SHOWWINDOW);

		bounds.left = SDL_windowX;
		bounds.top = SDL_windowY;
		bounds.right = SDL_windowX+width;
		bounds.bottom = SDL_windowY+height;
#ifndef _WIN32_WCE
		AdjustWindowRectEx(&bounds, GetWindowLong(SDL_Window, GWL_STYLE), (GetMenu(SDL_Window) != NULL), 0);
#else
		
		AdjustWindowRectEx(&bounds, GetWindowLong(SDL_Window, GWL_STYLE), 0, 0);
#endif
		width = bounds.right-bounds.left;
		height = bounds.bottom-bounds.top;
		if ( (flags & SDL_FULLSCREEN) ) {
			x = (GetSystemMetrics(SM_CXSCREEN)-width)/2;
			y = (GetSystemMetrics(SM_CYSCREEN)-height)/2;
		} else if ( center ) {
			x = (GetSystemMetrics(SM_CXSCREEN)-width)/2;
			y = (GetSystemMetrics(SM_CYSCREEN)-height)/2;
		} else if ( SDL_windowX || SDL_windowY || window ) {
			x = bounds.left;
			y = bounds.top;
		} else {
			x = y = -1;
			swp_flags |= SWP_NOMOVE;
		}
		if ( flags & SDL_FULLSCREEN ) {
			top = HWND_TOPMOST;
		} else {
			top = HWND_NOTOPMOST;
		}
		SetWindowPos(SDL_Window, top, x, y, width, height, swp_flags);
		if ( !(flags & SDL_FULLSCREEN) ) {
			SDL_windowX = SDL_bounds.left;
			SDL_windowY = SDL_bounds.top;
		}
		if ( GetParent(SDL_Window) == NULL ) {
			SetForegroundWindow(SDL_Window);
		}
	}
}

SDL_Surface *DIB_SetVideoMode(_THIS, SDL_Surface *current,
				int width, int height, int bpp, Uint32 flags)
{
	SDL_Surface *video;
	int prev_w, prev_h;
	Uint32 prev_flags;
	DWORD style;
	const DWORD directstyle =
			(WS_POPUP);
	const DWORD windowstyle = 
			(WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX);
	const DWORD resizestyle =
			(WS_THICKFRAME|WS_MAXIMIZEBOX);
	int binfo_size;
	BITMAPINFO *binfo;
	HDC hdc;
	Uint32 Rmask, Gmask, Bmask;

	prev_w = current->w;
	prev_h = current->h;
	prev_flags = current->flags;

	if ( SDL_Window &&
	     ((current->flags & ~SDL_ANYFORMAT) == (flags & ~SDL_ANYFORMAT)) &&
	     (current->format->BitsPerPixel == bpp) &&
	     (flags & SDL_OPENGL) && 
	     !(flags & SDL_FULLSCREEN) ) {  
		current->w = width;
		current->h = height;
		SDL_resizing = 1;
		DIB_ResizeWindow(this, width, height, prev_w, prev_h, flags);
		SDL_resizing = 0;
		return current;
	}

	
	if ( current->flags & SDL_OPENGL ) {
		WIN_GL_ShutDown(this);
	}
	SDL_resizing = 1;

	
	if ( bpp == current->format->BitsPerPixel ) {
		video = current;
	} else {
		switch (bpp) {
			case 15:
			case 16:
				if ( DIB_SussScreenDepth() == 15 ) {
					
					Rmask = 0x00007c00;
					Gmask = 0x000003e0;
					Bmask = 0x0000001f;
				} else {
					
					Rmask = 0x0000f800;
					Gmask = 0x000007e0;
					Bmask = 0x0000001f;
				}
				break;
			case 24:
			case 32:
				
				Rmask = 0x00ff0000;
				Gmask = 0x0000ff00;
				Bmask = 0x000000ff;
				break;
			default:
				Rmask = 0x00000000;
				Gmask = 0x00000000;
				Bmask = 0x00000000;
				break;
		}
		video = SDL_CreateRGBSurface(SDL_SWSURFACE,
					0, 0, bpp, Rmask, Gmask, Bmask, 0);
		if ( video == NULL ) {
			SDL_OutOfMemory();
			return(NULL);
		}
	}

	
	video->flags = 0;	
	video->w = width;
	video->h = height;
	video->pitch = SDL_CalculatePitch(video);

	SDL_VideoSurface = video;

#if defined(_WIN32_WCE)
	if ( flags & SDL_FULLSCREEN )
		video->flags |= SDL_FULLSCREEN;
#endif

#ifndef NO_CHANGEDISPLAYSETTINGS
	
	if ( (flags & SDL_FULLSCREEN) == SDL_FULLSCREEN ) {
		DEVMODE settings;
		BOOL changed;

		SDL_memset(&settings, 0, sizeof(DEVMODE));
		settings.dmSize = sizeof(DEVMODE);

#ifdef _WIN32_WCE
		
		if( this->hidden->supportRotation )
		{
			DWORD rotation;

			
			settings.dmFields = DM_DISPLAYORIENTATION;
			ChangeDisplaySettingsEx(NULL, &settings, NULL, CDS_TEST, NULL);
			rotation = settings.dmDisplayOrientation;

			if( (width > GetDeviceCaps(GetDC(NULL), HORZRES))
				&& (height < GetDeviceCaps(GetDC(NULL), VERTRES)))
			{
				switch( rotation )
				{
				case DMDO_0:
					settings.dmDisplayOrientation = DMDO_90;
					break;
				case DMDO_270:
					settings.dmDisplayOrientation = DMDO_180;
					break;
				}
				if( settings.dmDisplayOrientation != rotation )
				{
					
					this->hidden->origRotation = rotation;
					ChangeDisplaySettingsEx(NULL,&settings,NULL,CDS_RESET,NULL);
				}
			}
			if( (width < GetDeviceCaps(GetDC(NULL), HORZRES))
				&& (height > GetDeviceCaps(GetDC(NULL), VERTRES)))
			{
				switch( rotation )
				{
				case DMDO_90:
					settings.dmDisplayOrientation = DMDO_0;
					break;
				case DMDO_180:
					settings.dmDisplayOrientation = DMDO_270;
					break;
				}
				if( settings.dmDisplayOrientation != rotation )
				{
					
					this->hidden->origRotation = rotation;
					ChangeDisplaySettingsEx(NULL,&settings,NULL,CDS_RESET,NULL);
				}
			}

		}
#endif

#ifndef _WIN32_WCE
		settings.dmBitsPerPel = video->format->BitsPerPixel;
		settings.dmPelsWidth = width;
		settings.dmPelsHeight = height;
		settings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
		if ( width <= (int)SDL_desktop_mode.dmPelsWidth &&
		     height <= (int)SDL_desktop_mode.dmPelsHeight ) {
			settings.dmDisplayFrequency = SDL_desktop_mode.dmDisplayFrequency;
			settings.dmFields |= DM_DISPLAYFREQUENCY;
		}
		changed = (ChangeDisplaySettings(&settings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL);
		if ( ! changed && (settings.dmFields & DM_DISPLAYFREQUENCY) ) {
			settings.dmFields &= ~DM_DISPLAYFREQUENCY;
			changed = (ChangeDisplaySettings(&settings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL);
		}
#else
		changed = 1;
#endif
		if ( changed ) {
			video->flags |= SDL_FULLSCREEN;
			SDL_fullscreen_mode = settings;
		}

	}
#endif 

	
	if ( grab_palette ) {
		DIB_ReleaseStaticColors(SDL_Window);
		grab_palette = FALSE;
	}
	if ( screen_pal != NULL ) {
		DeleteObject(screen_pal);
		screen_pal = NULL;
	}
	if ( screen_logpal != NULL ) {
		SDL_free(screen_logpal);
		screen_logpal = NULL;
	}

	if ( bpp <= 8 )
	{
		DIB_CreatePalette(this, bpp);
	}

	style = GetWindowLong(SDL_Window, GWL_STYLE);
	style &= ~(resizestyle|WS_MAXIMIZE);
	if ( (video->flags & SDL_FULLSCREEN) == SDL_FULLSCREEN ) {
		style &= ~windowstyle;
		style |= directstyle;
	} else {
#ifndef NO_CHANGEDISPLAYSETTINGS
		if ( (prev_flags & SDL_FULLSCREEN) == SDL_FULLSCREEN ) {
			ChangeDisplaySettings(NULL, 0);
		}
#endif
		if ( flags & SDL_NOFRAME ) {
			style &= ~windowstyle;
			style |= directstyle;
			video->flags |= SDL_NOFRAME;
		} else {
			style &= ~directstyle;
			style |= windowstyle;
			if ( flags & SDL_RESIZABLE ) {
				style |= resizestyle;
				video->flags |= SDL_RESIZABLE;
			}
		}
#if WS_MAXIMIZE && !defined(_WIN32_WCE)
		if (IsZoomed(SDL_Window)) style |= WS_MAXIMIZE;
#endif
	}

	
	if ( !SDL_windowid )
		SetWindowLong(SDL_Window, GWL_STYLE, style);

	
	if ( screen_bmp != NULL ) {
		DeleteObject(screen_bmp);
	}
	if ( ! (flags & SDL_OPENGL) ) {
		BOOL is16bitmode = (video->format->BytesPerPixel == 2);

		
		binfo_size = sizeof(*binfo);
		if( is16bitmode ) {
			
			binfo_size += 3*sizeof(DWORD);
		} else if ( video->format->palette ) {
			binfo_size += video->format->palette->ncolors *
							sizeof(RGBQUAD);
		}
		binfo = (BITMAPINFO *)SDL_malloc(binfo_size);
		if ( ! binfo ) {
			if ( video != current ) {
				SDL_FreeSurface(video);
			}
			SDL_OutOfMemory();
			return(NULL);
		}

		binfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		binfo->bmiHeader.biWidth = video->w;
		binfo->bmiHeader.biHeight = -video->h;	
		binfo->bmiHeader.biPlanes = 1;
		binfo->bmiHeader.biSizeImage = video->h * video->pitch;
		binfo->bmiHeader.biXPelsPerMeter = 0;
		binfo->bmiHeader.biYPelsPerMeter = 0;
		binfo->bmiHeader.biClrUsed = 0;
		binfo->bmiHeader.biClrImportant = 0;
		binfo->bmiHeader.biBitCount = video->format->BitsPerPixel;

		if ( is16bitmode ) {
			
			binfo->bmiHeader.biCompression = BI_BITFIELDS;
			((Uint32*)binfo->bmiColors)[0] = video->format->Rmask;
			((Uint32*)binfo->bmiColors)[1] = video->format->Gmask;
			((Uint32*)binfo->bmiColors)[2] = video->format->Bmask;
		} else {
			binfo->bmiHeader.biCompression = BI_RGB;	
			if ( video->format->palette ) {
				SDL_memset(binfo->bmiColors, 0,
					video->format->palette->ncolors*sizeof(RGBQUAD));
			}
		}

		
		hdc = GetDC(SDL_Window);
		screen_bmp = CreateDIBSection(hdc, binfo, DIB_RGB_COLORS,
					(void **)(&video->pixels), NULL, 0);
		ReleaseDC(SDL_Window, hdc);
		SDL_free(binfo);
		if ( screen_bmp == NULL ) {
			if ( video != current ) {
				SDL_FreeSurface(video);
			}
			SDL_SetError("Couldn't create DIB section");
			return(NULL);
		}
		this->UpdateRects = DIB_NormalUpdate;

		
		if ( screen_pal && (flags & (SDL_FULLSCREEN|SDL_HWPALETTE)) ) {
			grab_palette = TRUE;
		}
		if ( screen_pal ) {
			
			video->flags |= SDL_HWPALETTE;
		}
	}
	DIB_ResizeWindow(this, width, height, prev_w, prev_h, flags);
	SDL_resizing = 0;

	
	if ( flags & SDL_OPENGL ) {
		if ( WIN_GL_SetupWindow(this) < 0 ) {
			return(NULL);
		}
		video->flags |= SDL_OPENGL;
	}

	WIN_FlushMessageQueue();

	
	return(video);
}

static int DIB_AllocHWSurface(_THIS, SDL_Surface *surface)
{
	return(-1);
}
static void DIB_FreeHWSurface(_THIS, SDL_Surface *surface)
{
	return;
}
static int DIB_LockHWSurface(_THIS, SDL_Surface *surface)
{
	return(0);
}
static void DIB_UnlockHWSurface(_THIS, SDL_Surface *surface)
{
	return;
}

static void DIB_NormalUpdate(_THIS, int numrects, SDL_Rect *rects)
{
	HDC hdc, mdc;
	int i;

	hdc = GetDC(SDL_Window);
	if ( screen_pal ) {
		SelectPalette(hdc, screen_pal, FALSE);
	}
	mdc = CreateCompatibleDC(hdc);
	SelectObject(mdc, screen_bmp);
	for ( i=0; i<numrects; ++i ) {
		BitBlt(hdc, rects[i].x, rects[i].y, rects[i].w, rects[i].h,
					mdc, rects[i].x, rects[i].y, SRCCOPY);
	}
	DeleteDC(mdc);
	ReleaseDC(SDL_Window, hdc);
}

static int FindPaletteIndex(LOGPALETTE *pal, BYTE r, BYTE g, BYTE b)
{
	PALETTEENTRY *entry;
	int i;
	int nentries = pal->palNumEntries;

	for ( i = 0; i < nentries; ++i ) {
		entry = &pal->palPalEntry[i];
		if ( entry->peRed == r && entry->peGreen == g && entry->peBlue == b ) {
			return i;
		}
	}
	return -1;
}

static BOOL CheckPaletteEntry(LOGPALETTE *pal, int index, BYTE r, BYTE g, BYTE b)
{
	PALETTEENTRY *entry;
	BOOL moved = 0;

	entry = &pal->palPalEntry[index];
	if ( entry->peRed != r || entry->peGreen != g || entry->peBlue != b ) {
		int found = FindPaletteIndex(pal, r, g, b);
		if ( found >= 0 ) {
			pal->palPalEntry[found] = *entry;
		}
		entry->peRed = r;
		entry->peGreen = g;
		entry->peBlue = b;
		moved = 1;
	}
	entry->peFlags = 0;

	return moved;
}

int DIB_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors)
{
#if !defined(_WIN32_WCE) || (_WIN32_WCE >= 400)
	HDC hdc, mdc;
	RGBQUAD *pal;
#else
	HDC hdc;
#endif
	int i;
	int moved_entries = 0;

	
	hdc = GetDC(SDL_Window);
	if ( screen_pal ) {
		PALETTEENTRY *entry;

		for ( i=0; i<ncolors; ++i ) {
			entry = &screen_logpal->palPalEntry[firstcolor+i];
			entry->peRed   = colors[i].r;
			entry->peGreen = colors[i].g;
			entry->peBlue  = colors[i].b;
			entry->peFlags = PC_NOCOLLAPSE;
		}
#if defined(SYSPAL_NOSTATIC) && !defined(_WIN32_WCE)
		
		if ( GetSystemPaletteUse(hdc) != SYSPAL_NOSTATIC256 ) {
			moved_entries += CheckPaletteEntry(screen_logpal, 0, 0x00, 0x00, 0x00);
			moved_entries += CheckPaletteEntry(screen_logpal, screen_logpal->palNumEntries-1, 0xff, 0xff, 0xff);
		}
#endif

#ifndef _WIN32_WCE
		
		UnrealizeObject(screen_pal);
#endif
		SetPaletteEntries(screen_pal, 0, screen_logpal->palNumEntries, screen_logpal->palPalEntry);
		SelectPalette(hdc, screen_pal, FALSE);
		RealizePalette(hdc);
	}

#if !defined(_WIN32_WCE) || (_WIN32_WCE >= 400)
	
	pal = SDL_stack_alloc(RGBQUAD, ncolors);
	for ( i=0; i<ncolors; ++i ) {
		pal[i].rgbRed = colors[i].r;
		pal[i].rgbGreen = colors[i].g;
		pal[i].rgbBlue = colors[i].b;
		pal[i].rgbReserved = 0;
	}

	
	mdc = CreateCompatibleDC(hdc);
	SelectObject(mdc, screen_bmp);
	SetDIBColorTable(mdc, firstcolor, ncolors, pal);
	if ( moved_entries || !grab_palette ) {
		BitBlt(hdc, 0, 0, this->screen->w, this->screen->h,
		       mdc, 0, 0, SRCCOPY);
	}
	DeleteDC(mdc);
	SDL_stack_free(pal);
#endif
	ReleaseDC(SDL_Window, hdc);
	return(1);
}


static void DIB_CheckGamma(_THIS)
{
#ifndef NO_GAMMA_SUPPORT
	HDC hdc;
	WORD ramp[3*256];

	
	hdc = GetDC(SDL_Window);
	if ( ! GetDeviceGammaRamp(hdc, ramp) ) {
		this->GetGammaRamp = NULL;
		this->SetGammaRamp = NULL;
	}
	ReleaseDC(SDL_Window, hdc);
#endif 
}
void DIB_SwapGamma(_THIS)
{
#ifndef NO_GAMMA_SUPPORT
	HDC hdc;

	if ( gamma_saved ) {
		hdc = GetDC(SDL_Window);
		if ( SDL_GetAppState() & SDL_APPINPUTFOCUS ) {
			
			SetDeviceGammaRamp(hdc, gamma_saved);
		} else {
			
			GetDeviceGammaRamp(hdc, gamma_saved);
			SetDeviceGammaRamp(hdc, this->gamma);
		}
		ReleaseDC(SDL_Window, hdc);
	}
#endif 
}
void DIB_QuitGamma(_THIS)
{
#ifndef NO_GAMMA_SUPPORT
	if ( gamma_saved ) {
		
		if ( SDL_GetAppState() & SDL_APPINPUTFOCUS ) {
			HDC hdc;

			hdc = GetDC(SDL_Window);
			SetDeviceGammaRamp(hdc, gamma_saved);
			ReleaseDC(SDL_Window, hdc);
		}

		
		SDL_free(gamma_saved);
		gamma_saved = 0;
	}
#endif 
}

int DIB_SetGammaRamp(_THIS, Uint16 *ramp)
{
#ifdef NO_GAMMA_SUPPORT
	SDL_SetError("SDL compiled without gamma ramp support");
	return -1;
#else
	HDC hdc;
	BOOL succeeded;

	
	if ( ! gamma_saved ) {
		gamma_saved = (WORD *)SDL_malloc(3*256*sizeof(*gamma_saved));
		if ( ! gamma_saved ) {
			SDL_OutOfMemory();
			return -1;
		}
		hdc = GetDC(SDL_Window);
		GetDeviceGammaRamp(hdc, gamma_saved);
		ReleaseDC(SDL_Window, hdc);
	}
	if ( SDL_GetAppState() & SDL_APPINPUTFOCUS ) {
		hdc = GetDC(SDL_Window);
		succeeded = SetDeviceGammaRamp(hdc, ramp);
		ReleaseDC(SDL_Window, hdc);
	} else {
		succeeded = TRUE;
	}
	return succeeded ? 0 : -1;
#endif 
}

int DIB_GetGammaRamp(_THIS, Uint16 *ramp)
{
#ifdef NO_GAMMA_SUPPORT
	SDL_SetError("SDL compiled without gamma ramp support");
	return -1;
#else
	HDC hdc;
	BOOL succeeded;

	
	hdc = GetDC(SDL_Window);
	succeeded = GetDeviceGammaRamp(hdc, ramp);
	ReleaseDC(SDL_Window, hdc);
	return succeeded ? 0 : -1;
#endif 
}

void DIB_VideoQuit(_THIS)
{
	int i, j;

	
	if ( SDL_Window ) {
		
		if ( this->screen ) {
			if ( grab_palette ) {
				DIB_ReleaseStaticColors(SDL_Window);
			}
#ifndef NO_CHANGEDISPLAYSETTINGS
			if ( this->screen->flags & SDL_FULLSCREEN ) {
				ChangeDisplaySettings(NULL, 0);
				ShowWindow(SDL_Window, SW_HIDE);
			}
#endif
			if ( this->screen->flags & SDL_OPENGL ) {
				WIN_GL_ShutDown(this);
			}
			this->screen->pixels = NULL;
		}
		if ( screen_pal != NULL ) {
			DeleteObject(screen_pal);
			screen_pal = NULL;
		}
		if ( screen_logpal != NULL ) {
			SDL_free(screen_logpal);
			screen_logpal = NULL;
		}
		if ( screen_bmp ) {
			DeleteObject(screen_bmp);
			screen_bmp = NULL;
		}
		if ( screen_icn ) {
			DestroyIcon(screen_icn);
			screen_icn = NULL;
		}
		DIB_QuitGamma(this);
		DIB_DestroyWindow(this);

		SDL_Window = NULL;

#if defined(_WIN32_WCE)

		if( aygshell ) 
		{
			FreeLibrary(aygshell);
			aygshell = NULL;
		}
#endif
	}

	for ( i=0; i < SDL_arraysize(SDL_modelist); ++i ) {
		if ( !SDL_modelist[i] ) {
			continue;
		}
		for ( j=0; SDL_modelist[i][j]; ++j ) {
			SDL_free(SDL_modelist[i][j]);
		}
		SDL_free(SDL_modelist[i]);
		SDL_modelist[i] = NULL;
		SDL_nummodes[i] = 0;
	}
}

static void DIB_GrabStaticColors(HWND window)
{
#if defined(SYSPAL_NOSTATIC) && !defined(_WIN32_WCE)
	HDC hdc;

	hdc = GetDC(window);
	SetSystemPaletteUse(hdc, SYSPAL_NOSTATIC256);
	if ( GetSystemPaletteUse(hdc) != SYSPAL_NOSTATIC256 ) {
		SetSystemPaletteUse(hdc, SYSPAL_NOSTATIC);
	}
	ReleaseDC(window, hdc);
#endif
}
static void DIB_ReleaseStaticColors(HWND window)
{
#if defined(SYSPAL_NOSTATIC) && !defined(_WIN32_WCE)
	HDC hdc;

	hdc = GetDC(window);
	SetSystemPaletteUse(hdc, SYSPAL_STATIC);
	ReleaseDC(window, hdc);
#endif
}
static void DIB_Activate(_THIS, BOOL active, BOOL minimized)
{
	if ( grab_palette ) {
		if ( !active ) {
			DIB_ReleaseStaticColors(SDL_Window);
			DIB_RealizePalette(this);
		} else if ( !minimized ) {
			DIB_GrabStaticColors(SDL_Window);
			DIB_RealizePalette(this);
		}
	}
}
static void DIB_RealizePalette(_THIS)
{
	if ( screen_pal != NULL ) {
		HDC hdc;

		hdc = GetDC(SDL_Window);
#ifndef _WIN32_WCE
		UnrealizeObject(screen_pal);
#endif
		SelectPalette(hdc, screen_pal, FALSE);
		if ( RealizePalette(hdc) ) {
			InvalidateRect(SDL_Window, NULL, FALSE);
		}
		ReleaseDC(SDL_Window, hdc);
	}
}
static void DIB_PaletteChanged(_THIS, HWND window)
{
	if ( window != SDL_Window ) {
		DIB_RealizePalette(this);
	}
}

static void DIB_WinPAINT(_THIS, HDC hdc)
{
	HDC mdc;

	if ( screen_pal ) {
		SelectPalette(hdc, screen_pal, FALSE);
	}
	mdc = CreateCompatibleDC(hdc);
	SelectObject(mdc, screen_bmp);
	BitBlt(hdc, 0, 0, SDL_VideoSurface->w, SDL_VideoSurface->h,
							mdc, 0, 0, SRCCOPY);
	DeleteDC(mdc);
}

static void DIB_GetWinPos(_THIS, int* px, int *py)
{
	RECT  rect;
	GetWindowRect(SDL_Window, &rect);
	*px = rect.left;
	*py = rect.top;
}

static void DIB_SetWinPos(_THIS, int  x, int  y)
{
    SetWindowPos(SDL_Window, HWND_TOPMOST, 
                 x, y, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
}

typedef struct {
    int   result;
    int   first;
    RECT  wrect;
    RECT  primary;
} VisibilityData;


BOOL CALLBACK visibility_cb(HMONITOR  hMonitor,
                            HDC       hdcMonitor,
                            LPRECT    mrect,
                            LPARAM    dwData)
{
    VisibilityData*  data = (VisibilityData*)dwData;
    
    if ( data->first ) {
        data->first   = 0;
        data->primary = mrect[0];
    }
    
    if ( data->wrect.left   >= mrect->left   &&
         data->wrect.right  <= mrect->right  &&
         data->wrect.top    >= mrect->top    &&
         data->wrect.bottom <= mrect->bottom )
    {
        data->result = 1;
        return FALSE;
    }
    return TRUE;
}

static int  DIB_IsWinVisible(_THIS, int  recenter)
{
    VisibilityData  data;
    data.result = 0;
    data.first  = 1;
    
    GetWindowRect(SDL_Window, &data.wrect);

    EnumDisplayMonitors(NULL, NULL, visibility_cb, (LPARAM)&data);
    
    if ( !data.result && recenter ) {
        int  new_x = 10;
        int  new_y = 10;

        if ( !data.first ) {
            int  primary_w = data.primary.right  - data.primary.left;
            int  primary_h = data.primary.bottom - data.primary.top;

            new_x = data.primary.left + (primary_w - this->screen->w)/2;
            new_y = data.primary.top  + (primary_h - this->screen->h)/2;
        }
        DIB_SetWinPos(this, new_x, new_y);
    }
    return  data.result;
}

static int  DIB_GetMonitorDPI(_THIS, int* xDpi, int *yDpi)
{
    HDC  displayDC = CreateDC( "DISPLAY", NULL, NULL, NULL );
    int  xdpi, ydpi;

    if (displayDC == NULL) {
        return -1;
    }
    xdpi = GetDeviceCaps( displayDC, LOGPIXELSX );
    ydpi = GetDeviceCaps( displayDC, LOGPIXELSY );

    DeleteDC(displayDC);

    
    if (xdpi < 20 || xdpi > 400 || ydpi < 20 || ydpi > 400) {
        return -1;
    }

    *xDpi = xdpi;
    *yDpi = ydpi;
    return 0;
}


typedef struct {
    int   first;
    RECT  wrect;
	long  bestArea;
	RECT  bestRect;
	RECT  primary;
} ProximityData;

BOOL CALLBACK proximity_cb(HMONITOR  hMonitor,
                           HDC       hdcMonitor,
                           LPRECT    mrect,
                           LPARAM    dwData)
{
    ProximityData*  data = (ProximityData*)dwData;
    int   x1, y1, x2, y2, area;
	
	x1 = mrect->left;
	x2 = mrect->right;
	y1 = mrect->top;
	y2 = mrect->bottom;

	if (data->first) {
		data->primary = mrect[0];
	}
	
	if (x1 < data->wrect.left)
		x1 = data->wrect.left;
    if (x2 > data->wrect.right)
		x2 = data->wrect.right;
	if (y1 < data->wrect.top)
		y1 = data->wrect.top;
	if (y2 > data->wrect.bottom)
		y2 = data->wrect.bottom;
	
	if (x1 >= x2 || y1 >= y2)
		return TRUE;

	area = (x2-x1)*(y2-y1);
	if (data->first || area > data->bestArea) {
		data->first    = 0;
		data->bestRect = mrect[0];
		data->bestArea = area;
	}
    return TRUE;
}

static int  DIB_GetMonitorRect(_THIS, SDL_Rect*  rect)
{
    ProximityData  data;
	RECT*          sr;

    data.first  = 1;
    GetWindowRect(SDL_Window, &data.wrect);

    EnumDisplayMonitors(NULL, NULL, proximity_cb, (LPARAM)&data);

	if (data.first)
		return -1;

	sr = &data.bestRect;

	rect->x = sr->left;
	rect->y = sr->top;
	rect->w = sr->right - sr->left;
	rect->h = sr->bottom - sr->top;
	
	return 0;
}

#if !SDL_AUDIO_DRIVER_DSOUND
void DX5_SoundFocus(HWND hwnd)
{
	return;
}
#endif

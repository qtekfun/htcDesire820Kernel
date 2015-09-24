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





#include "SDL.h"
#include "SDL_error.h"
#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"
#include "../wincommon/SDL_syswm_c.h"
#include "../wincommon/SDL_sysmouse_c.h"
#include "../windib/SDL_dibevents_c.h" 

#include "../windib/SDL_gapidibvideo.h"
#include "SDL_gapivideo.h"

#define gapi this->hidden->gapiInfo

#define GAPIVID_DRIVER_NAME "gapi"

#if defined(DEBUG) || defined (_DEBUG) || defined(NDEBUG)
#define REPORT_VIDEO_INFO 1
#else
#define REPORT_VIDEO_INFO 0
#endif

#define USE_GAPI_EMU 0
#define EMULATE_AXIM_X30 0
#define WITHOUT_GAPI 0

#if USE_GAPI_EMU && !REPORT_VIDEO_INFO
#pragma message("Warning: Using GapiEmu in release build. I assume you'd like to set USE_GAPI_EMU to zero.")
#endif

#ifndef _T
#define _T(x) L##x
#endif

#ifndef ASSERT
#define ASSERT(x)
#endif

extern HINSTANCE aygshell;
extern void SDL_UnregisterApp();
extern int DIB_AddMode(_THIS, int bpp, int w, int h);

static int GAPI_VideoInit(_THIS, SDL_PixelFormat *vformat);
static SDL_Rect **GAPI_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags);
static SDL_Surface *GAPI_SetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags);
static int GAPI_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors);
static void GAPI_VideoQuit(_THIS);

static int GAPI_AllocHWSurface(_THIS, SDL_Surface *surface);
static int GAPI_LockHWSurface(_THIS, SDL_Surface *surface);
static void GAPI_UnlockHWSurface(_THIS, SDL_Surface *surface);
static void GAPI_FreeHWSurface(_THIS, SDL_Surface *surface);

static void GAPI_Activate(_THIS, BOOL active, BOOL minimized);
static void GAPI_RealizePalette(_THIS);
static void GAPI_PaletteChanged(_THIS, HWND window);
static void GAPI_WinPAINT(_THIS, HDC hdc); 

static void GAPI_UpdateRects(_THIS, int numrects, SDL_Rect *rects);

static HMODULE g_hGapiLib = 0;
#define LINK(type,name,import) \
	if( g_hGapiLib ) \
		name = (PFN##type)GetProcAddress( g_hGapiLib, _T(import) ); 

static char g_bRawBufferAvailable = 0;


typedef struct _RawFrameBufferInfo
{
   WORD wFormat;
   WORD wBPP;
   VOID *pFramePointer;
   int  cxStride;
   int  cyStride;
   int  cxPixels;
   int  cyPixels;
} RawFrameBufferInfo; 

static struct _RawFrameBufferInfo g_RawFrameBufferInfo = {0};

#define GETRAWFRAMEBUFFER   0x00020001

#define FORMAT_565 1
#define FORMAT_555 2
#define FORMAT_OTHER 3

#define GETGXINFO 0x00020000

typedef struct GXDeviceInfo
{
long Version; 
void * pvFrameBuffer; 
unsigned long cbStride; 
unsigned long cxWidth; 
unsigned long cyHeight; 
unsigned long cBPP; 
unsigned long ffFormat; 
char Unused[0x84-7*4];
} GXDeviceInfo; 

static int GAPI_Available(void)
{
	
	HDC hdc = GetDC(NULL); 
	int result = ExtEscape(hdc, GETRAWFRAMEBUFFER, 0, NULL, sizeof(RawFrameBufferInfo), (char *)&g_RawFrameBufferInfo);
	ReleaseDC(NULL, hdc);
	g_bRawBufferAvailable = result > 0;

	
	if(g_RawFrameBufferInfo.cxPixels <= 0 || g_RawFrameBufferInfo.cyPixels <= 0){
		g_bRawBufferAvailable = 0;
	}

#if WITHOUT_GAPI
	return g_bRawBufferAvailable;
#endif

#if USE_GAPI_EMU
	g_hGapiLib = LoadLibrary(_T("GAPI_Emu.dll"));
	if( !g_hGapiLib )
	{
		SDL_SetError("Gapi Emu not found!");
	}
	return g_hGapiLib != 0;
#endif

	
	g_hGapiLib = LoadLibrary(_T("\\Windows\\gx.dll"));
	if( !g_hGapiLib )
	{
		g_hGapiLib = LoadLibrary(_T("gx.dll"));
		if( !g_hGapiLib ) return g_bRawBufferAvailable;
	}

	return(1);
}

static int cmpmodes(const void *va, const void *vb)
{
    SDL_Rect *a = *(SDL_Rect **)va;
    SDL_Rect *b = *(SDL_Rect **)vb;
    if ( a->w == b->w )
        return b->h - a->h;
    else
        return b->w - a->w;
}

static int GAPI_AddMode(_THIS, int bpp, int w, int h)
{
	SDL_Rect *mode;
	int i, index;
	int next_mode;

	
	if ( bpp < 8 ) {  
		return(0);
	}
	index = ((bpp+7)/8)-1;
	for ( i=0; i<gapi->SDL_nummodes[index]; ++i ) {
		mode = gapi->SDL_modelist[index][i];
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

	
	next_mode = gapi->SDL_nummodes[index];
	gapi->SDL_modelist[index] = (SDL_Rect **)
	       SDL_realloc(gapi->SDL_modelist[index], (1+next_mode+1)*sizeof(SDL_Rect *));
	if ( gapi->SDL_modelist[index] == NULL ) {
		SDL_OutOfMemory();
		gapi->SDL_nummodes[index] = 0;
		SDL_free(mode);
		return(-1);
	}
	gapi->SDL_modelist[index][next_mode] = mode;
	gapi->SDL_modelist[index][next_mode+1] = NULL;
	gapi->SDL_nummodes[index]++;

	return(0);
}

static void GAPI_DeleteDevice(SDL_VideoDevice *device)
{
	if( g_hGapiLib )
	{
		FreeLibrary(g_hGapiLib);
		g_hGapiLib = 0;
	}
	SDL_free(device->hidden->gapiInfo);
	SDL_free(device->hidden);
	SDL_free(device);
}

static SDL_VideoDevice *GAPI_CreateDevice(int devindex)
{
	SDL_VideoDevice *device;

	if( !g_hGapiLib && !g_bRawBufferAvailable)
	{
		if( !GAPI_Available() )
		{
			SDL_SetError("GAPI dll is not found and VGA mode is not available!");
			return 0;
		}
	}

	
	device = (SDL_VideoDevice *)SDL_malloc(sizeof(SDL_VideoDevice));
	if ( device ) {
		SDL_memset(device, 0, (sizeof *device));
		device->hidden = (struct SDL_PrivateVideoData *)
				SDL_malloc((sizeof *device->hidden));
		if(device->hidden){
			SDL_memset(device->hidden, 0, (sizeof *device->hidden));
			device->hidden->gapiInfo = (GapiInfo *)SDL_malloc((sizeof(GapiInfo)));
			if(device->hidden->gapiInfo == NULL)
			{
				SDL_free(device->hidden);
				device->hidden = NULL;
			}
		}
	}
	if ( (device == NULL) || (device->hidden == NULL) ) {
		SDL_OutOfMemory();
		if ( device ) {
			SDL_free(device);
		}
		return(0);
	}
	SDL_memset(device->hidden->gapiInfo, 0, (sizeof *device->hidden->gapiInfo));

	
	device->VideoInit = GAPI_VideoInit;
	device->ListModes = GAPI_ListModes;
	device->SetVideoMode = GAPI_SetVideoMode;
	device->UpdateMouse = WIN_UpdateMouse; 
	device->CreateYUVOverlay = NULL;
	device->SetColors = GAPI_SetColors;
	device->UpdateRects = GAPI_UpdateRects;
	device->VideoQuit = GAPI_VideoQuit;
	device->AllocHWSurface = GAPI_AllocHWSurface;
	device->CheckHWBlit = NULL;
	device->FillHWRect = NULL;
	device->SetHWColorKey = NULL;
	device->SetHWAlpha = NULL;
	device->LockHWSurface = GAPI_LockHWSurface;
	device->UnlockHWSurface = GAPI_UnlockHWSurface;
	device->FlipHWSurface = NULL;
	device->FreeHWSurface = GAPI_FreeHWSurface;
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

	
	WIN_Activate = GAPI_Activate;
	WIN_RealizePalette = GAPI_RealizePalette;
	WIN_PaletteChanged = GAPI_PaletteChanged;
	WIN_WinPAINT = GAPI_WinPAINT;
	HandleMessage = DIB_HandleMessage; 

	device->free = GAPI_DeleteDevice;

	
#define gx device->hidden->gapiInfo->gxFunc

    LINK( GXOpenDisplay, gx.GXOpenDisplay,         "?GXOpenDisplay@@YAHPAUHWND__@@K@Z" )
    LINK( GXCloseDisplay, gx.GXCloseDisplay,        "?GXCloseDisplay@@YAHXZ" )
    LINK( GXBeginDraw, gx.GXBeginDraw,           "?GXBeginDraw@@YAPAXXZ" )
    LINK( GXEndDraw, gx.GXEndDraw,             "?GXEndDraw@@YAHXZ" )
    LINK( GXOpenInput, gx.GXOpenInput,           "?GXOpenInput@@YAHXZ" )
    LINK( GXCloseInput, gx.GXCloseInput,          "?GXCloseInput@@YAHXZ" )
    LINK( GXGetDisplayProperties, gx.GXGetDisplayProperties,"?GXGetDisplayProperties@@YA?AUGXDisplayProperties@@XZ" )
    LINK( GXGetDefaultKeys, gx.GXGetDefaultKeys,      "?GXGetDefaultKeys@@YA?AUGXKeyList@@H@Z" )
    LINK( GXSuspend, gx.GXSuspend,             "?GXSuspend@@YAHXZ" )
    LINK( GXResume, gx.GXResume,              "?GXResume@@YAHXZ" )
    LINK( GXSetViewport, gx.GXSetViewport,         "?GXSetViewport@@YAHKKKK@Z" )
    LINK( GXIsDisplayDRAMBuffer, gx.GXIsDisplayDRAMBuffer, "?GXIsDisplayDRAMBuffer@@YAHXZ" )

	
	if( !gx.GXOpenDisplay )
	{
		if( g_hGapiLib ) 
		{
			FreeLibrary(g_hGapiLib);
			g_hGapiLib = 0;
		}
	}
	
	if( !gx.GXOpenDisplay && !g_bRawBufferAvailable)
	{
		SDL_SetError("Error: damaged or unknown gapi.dll!\n");
		GAPI_DeleteDevice(device);
		return 0;
	}

	return device;
}

VideoBootStrap GAPI_bootstrap = {
	GAPIVID_DRIVER_NAME, "WinCE GAPI video driver",
	GAPI_Available, GAPI_CreateDevice
};

static void FillStructs(_THIS, BOOL useVga)
{
#ifdef _ARM_
	WCHAR oemstr[100];
#endif
	

	if( !useVga )
	{
		gapi->gxProperties = gapi->gxFunc.GXGetDisplayProperties();
		gapi->needUpdate = 1;
		gapi->hiresFix = 0;
		gapi->useVga = 0;
		gapi->useGXOpenDisplay = 1;

#ifdef _ARM_
		
		SystemParametersInfo( SPI_GETOEMINFO, sizeof( oemstr ), oemstr, 0 );

		
		if ((oemstr[12] == 'H') && (oemstr[13] == '3') && (oemstr[14] == '8') && (gapi->gxProperties.cbxPitch > 0)) 
		{
			gapi->videoMem = (PIXEL*)0xac0755a0;
			gapi->gxProperties.cbxPitch = -640;
			gapi->gxProperties.cbyPitch = 2;
			gapi->needUpdate = 0;
		}
#if (EMULATE_AXIM_X30 == 0)
		
		if( _tcsncmp(oemstr, L"Dell Axim X30", 13) == 0 )
#endif
		{
			GXDeviceInfo gxInfo = {0};
			HDC hdc = GetDC(NULL);
			int result;

			gxInfo.Version = 100;
			result = ExtEscape(hdc, GETGXINFO, 0, NULL, sizeof(gxInfo), (char *)&gxInfo);
			if( result > 0 )
			{
				gapi->useGXOpenDisplay = 0;
				gapi->videoMem = gxInfo.pvFrameBuffer;
				gapi->needUpdate = 0;
				gapi->gxProperties.cbxPitch = 2;
				gapi->gxProperties.cbyPitch = 480;
				gapi->gxProperties.cxWidth = gxInfo.cxWidth;
				gapi->gxProperties.cyHeight = gxInfo.cyHeight;
				gapi->gxProperties.ffFormat = gxInfo.ffFormat;
			}
		}
#endif
	} else
	{
		gapi->needUpdate = 0;		
		gapi->hiresFix = 0;
		gapi->gxProperties.cBPP = g_RawFrameBufferInfo.wBPP;
		gapi->gxProperties.cbxPitch = g_RawFrameBufferInfo.cxStride;
		gapi->gxProperties.cbyPitch = g_RawFrameBufferInfo.cyStride;
		gapi->gxProperties.cxWidth = g_RawFrameBufferInfo.cxPixels;
		gapi->gxProperties.cyHeight = g_RawFrameBufferInfo.cyPixels;
		gapi->videoMem = g_RawFrameBufferInfo.pFramePointer;
		gapi->useVga = 1;

		switch( g_RawFrameBufferInfo.wFormat )
		{
		case FORMAT_565:
			gapi->gxProperties.ffFormat = kfDirect565;
			break;
		case FORMAT_555:
			gapi->gxProperties.ffFormat = kfDirect555;
			break;
		default:
			
			switch( g_RawFrameBufferInfo.wBPP )
			{
			case 4:
			case 8:
				gapi->gxProperties.ffFormat = kfDirect;
			case 16:
				gapi->gxProperties.ffFormat = kfDirect565;
			default:
				gapi->gxProperties.ffFormat = kfDirect;
				break;
			}
		}
	}

	if( gapi->gxProperties.cBPP != 16 )
	{
		gapi->gapiOrientation = SDL_ORIENTATION_UP;
	} else
	if( (gapi->gxProperties.cbxPitch > 0) && (gapi->gxProperties.cbyPitch > 0 ))
	{
		gapi->gapiOrientation = SDL_ORIENTATION_UP;
	} else
	if( (gapi->gxProperties.cbxPitch > 0) && (gapi->gxProperties.cbyPitch < 0 ))
	{
		gapi->gapiOrientation = SDL_ORIENTATION_RIGHT; 
	} else
	if( (gapi->gxProperties.cbxPitch < 0) && (gapi->gxProperties.cbyPitch > 0 ))
	{
		gapi->gapiOrientation = SDL_ORIENTATION_LEFT; 
	}
}

static void GAPI_CreatePalette(int ncolors, SDL_Color *colors)
{
  
   BYTE buffer[ sizeof(LOGPALETTE) + 255 * sizeof(PALETTEENTRY) ];
   int i;
   LOGPALETTE*   pLogical = (LOGPALETTE*)buffer;
   PALETTEENTRY* entries  = pLogical->palPalEntry;
   HPALETTE hPalette;
   HDC hdc;

   for (i = 0; i < ncolors; ++i)
   {
      
      entries[i].peRed   = colors[i].r;
      entries[i].peGreen = colors[i].g;
      entries[i].peBlue  = colors[i].b;
      entries[i].peFlags = 0;
   }

   
   pLogical->palVersion    = 0x0300;
   pLogical->palNumEntries = ncolors;

   hPalette = CreatePalette( pLogical );
   ASSERT(hPalette);
	

   
   hdc = GetDC(0);

   SelectPalette( hdc, hPalette, FALSE );
   RealizePalette( hdc );

   ReleaseDC( 0, hdc );
   DeleteObject( hPalette );
}

int GAPI_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
	int i,bpp;

	
	if ( DIB_CreateWindow(this) < 0 ) {
		return(-1);
	}

	if( g_hGapiLib )
	{
		FillStructs(this, 0);

		
		bpp = gapi->gxProperties.cBPP < 8 ? 16 : gapi->gxProperties.cBPP;
		
		
		GAPI_AddMode(this, bpp, gapi->gxProperties.cyHeight, gapi->gxProperties.cxWidth);	
		GAPI_AddMode(this, bpp, gapi->gxProperties.cxWidth, gapi->gxProperties.cyHeight);	
	}

	
	if( g_bRawBufferAvailable && 
		!((gapi->gxProperties.cxWidth == (unsigned)g_RawFrameBufferInfo.cxPixels) && (gapi->gxProperties.cyHeight == (unsigned)g_RawFrameBufferInfo.cyPixels)))
	{
		FillStructs(this, 1);

		
		bpp = gapi->gxProperties.cBPP < 8 ? 16 : gapi->gxProperties.cBPP;

		
		GAPI_AddMode(this, bpp, gapi->gxProperties.cyHeight, gapi->gxProperties.cxWidth);	
		GAPI_AddMode(this, bpp, gapi->gxProperties.cxWidth, gapi->gxProperties.cyHeight);	
	}

	this->info.current_w = GetSystemMetrics(SM_CXSCREEN);
	this->info.current_h = GetSystemMetrics(SM_CYSCREEN);

	
	for ( i=0; i<NUM_MODELISTS; ++i ) {
		if ( gapi->SDL_nummodes[i] > 0 ) {
			SDL_qsort(gapi->SDL_modelist[i], gapi->SDL_nummodes[i], sizeof *gapi->SDL_modelist[i], cmpmodes);
		}
	}

	vformat->BitsPerPixel = gapi->gxProperties.cBPP < 8 ? 16 : (unsigned char)gapi->gxProperties.cBPP;

	
	if (gapi->gxProperties.ffFormat & kfDirect565) {
		vformat->BitsPerPixel = 16;
		vformat->Rmask = 0x0000f800;
		vformat->Gmask = 0x000007e0;
		vformat->Bmask = 0x0000001f;
		gapi->videoMode = GAPI_DIRECT_565;
	}
	else
	if (gapi->gxProperties.ffFormat & kfDirect555) {
		vformat->BitsPerPixel = 16;
		vformat->Rmask = 0x00007c00;
		vformat->Gmask = 0x000003e0;
		vformat->Bmask = 0x0000001f;
		gapi->videoMode = GAPI_DIRECT_555;
	}
	else
	if ((gapi->gxProperties.ffFormat & kfDirect) && (gapi->gxProperties.cBPP < 8)) {
		
		vformat->BitsPerPixel = 16;
		vformat->Rmask = 0x0000f800; 
		vformat->Gmask = 0x000007e0;
		vformat->Bmask = 0x0000001f;
		if (gapi->gxProperties.ffFormat & kfDirectInverted)
			gapi->invert = (1 << gapi->gxProperties.cBPP) - 1;
		gapi->colorscale = gapi->gxProperties.cBPP < 8 ? 8 - gapi->gxProperties.cBPP : 0;
		gapi->videoMode = GAPI_MONO;
	}
	else
	if (gapi->gxProperties.ffFormat & kfPalette) {
		gapi->videoMode = GAPI_PALETTE;
	} 

	
	return(0);
}

SDL_Rect **GAPI_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags)
{
	return(gapi->SDL_modelist[((format->BitsPerPixel+7)/8)-1]);
}

SDL_Surface *GAPI_SetVideoMode(_THIS, SDL_Surface *current,
				int width, int height, int bpp, Uint32 flags)
{
	SDL_Surface *video; 
	Uint32 Rmask, Gmask, Bmask; 
	DWORD style; 
	SDL_Rect allScreen;

	if( bpp < 4 )
	{
		SDL_SetError("1 bpp and 2 bpp modes is not implemented yet!");
		return 0;
	}

	
	if (bpp == current->format->BitsPerPixel) {
		video = current;
	}
	else {
		switch(bpp) {
			case 8:
				Rmask = 0;
				Gmask = 0;
				Bmask = 0;
				break;
			case 15:				
			case 16:
				
				if (gapi->gxProperties.ffFormat & kfDirect555) {
					Rmask = 0x00007c00;
					Gmask = 0x000003e0;
					Bmask = 0x0000001f;
				}
				else {
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
				SDL_SetError("Unsupported Bits Per Pixel format requested");
				return NULL;
		}
		video = SDL_CreateRGBSurface(SDL_SWSURFACE,
					0, 0, bpp, Rmask, Gmask, Bmask, 0);
		if ( video == NULL ) {
			SDL_OutOfMemory();
			return(NULL);
		}
	}

	gapi->userOrientation = SDL_ORIENTATION_UP;
	gapi->systemOrientation = SDL_ORIENTATION_UP;
	video->flags = SDL_FULLSCREEN;	

	
	if( g_hGapiLib )
	{
		FillStructs(this, 0);
		if( (((unsigned)width != gapi->gxProperties.cxWidth) || ((unsigned)height != gapi->gxProperties.cyHeight))
			&& (((unsigned)width != gapi->gxProperties.cyHeight) || ((unsigned)height != gapi->gxProperties.cxWidth)))
			FillStructs(this, 1); 
	} else
		FillStructs(this, 1);

	if ( !gapi->needUpdate && !gapi->videoMem) {
		SDL_SetError("Couldn't get address of video memory, may be unsupported device or bug");
		return(NULL);
	}

	
       if( (width > height) && (gapi->gxProperties.cxWidth < gapi->gxProperties.cyHeight))
		gapi->userOrientation = SDL_ORIENTATION_RIGHT;

       if(GetSystemMetrics(SM_CYSCREEN) < GetSystemMetrics(SM_CXSCREEN))
		gapi->systemOrientation = SDL_ORIENTATION_RIGHT;

	gapi->hiresFix = 0;

	
	if(GetSystemMetrics(SM_CXSCREEN) < width && GetSystemMetrics(SM_CYSCREEN) < height)
	{
	    gapi->hiresFix = 1;
	}

	switch( gapi->userOrientation )
	{
	case SDL_ORIENTATION_UP:
		gapi->startOffset = 0;
		gapi->dstLineStep = gapi->gxProperties.cbyPitch;
		gapi->dstPixelStep = gapi->gxProperties.cbxPitch;
		break;
	case SDL_ORIENTATION_RIGHT:
		switch( gapi->gapiOrientation )
		{
		case SDL_ORIENTATION_UP:
		case SDL_ORIENTATION_RIGHT:
		case SDL_ORIENTATION_LEFT:
			if( (gapi->videoMode == GAPI_MONO) )
				gapi->startOffset = -gapi->gxProperties.cbxPitch + 1; 
			else
				gapi->startOffset = gapi->gxProperties.cbyPitch * (gapi->gxProperties.cyHeight - 1);
				
			gapi->dstLineStep = gapi->gxProperties.cbxPitch;
			gapi->dstPixelStep = -gapi->gxProperties.cbyPitch;
			break;
		}
	}

	video->w = gapi->w = width;
	video->h = gapi->h = height;
	video->pitch = SDL_CalculatePitch(video); 

	SDL_VideoSurface = video;

	
	style = 0;

	if (!SDL_windowid)
		SetWindowLong(SDL_Window, GWL_STYLE, style);

	
	if( gapi->buffer ) 
	{
		SDL_free( gapi->buffer );
		gapi->buffer = NULL;
	}
	gapi->buffer = SDL_malloc(video->h * video->pitch);
	video->pixels = gapi->buffer; 

	if ( ! gapi->buffer ) {
		SDL_SetError("Couldn't allocate buffer for requested mode");
		return(NULL);
	}

	SDL_memset(gapi->buffer, 255, video->h * video->pitch);
	MoveWindow(SDL_Window, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), FALSE);
	ShowWindow(SDL_Window, SW_SHOW);
	SetForegroundWindow(SDL_Window);

	WIN_FlushMessageQueue();

       
       if( !gapi->useVga && gapi->useGXOpenDisplay && !gapi->alreadyGXOpened )
       {
#if REPORT_VIDEO_INFO
               printf("system display width  (orig): %d\n", GetSystemMetrics(SM_CXSCREEN));
               printf("system display height (orig): %d\n", GetSystemMetrics(SM_CYSCREEN));
#endif
               gapi->alreadyGXOpened = 1;
		if( !gapi->gxFunc.GXOpenDisplay(SDL_Window, GX_FULLSCREEN) )
		{
			SDL_SetError("Couldn't initialize GAPI");
			return(NULL);
		}
       }

	if(gapi->useVga)
		gapi->coordinateTransform = (4 - gapi->systemOrientation + gapi->userOrientation) % 4;
	else
		gapi->coordinateTransform = gapi->userOrientation;

#if REPORT_VIDEO_INFO
	printf("Video properties:\n");
	printf("display bpp: %d\n", gapi->gxProperties.cBPP);
	printf("display width: %d\n", gapi->gxProperties.cxWidth);
	printf("display height: %d\n", gapi->gxProperties.cyHeight);
       printf("system display width: %d\n", GetSystemMetrics(SM_CXSCREEN));
       printf("system display height: %d\n", GetSystemMetrics(SM_CYSCREEN));
	printf("x pitch: %d\n", gapi->gxProperties.cbxPitch);
	printf("y pitch: %d\n", gapi->gxProperties.cbyPitch);
	printf("gapi flags: 0x%x\n", gapi->gxProperties.ffFormat);
       printf("user orientation: %d\n", gapi->userOrientation);
	printf("system orientation: %d\n", gapi->systemOrientation);
       printf("gapi orientation: %d\n", gapi->gapiOrientation);


	if( !gapi->useVga && gapi->useGXOpenDisplay && gapi->needUpdate)
	{
		gapi->videoMem = gapi->gxFunc.GXBeginDraw(); 
		gapi->gxFunc.GXEndDraw();
	}

	printf("video memory: 0x%x\n", gapi->videoMem);
	printf("need update: %d\n", gapi->needUpdate);
	printf("hi-res fix: %d\n", gapi->hiresFix);
	printf("VGA is available on the device: %d\n", g_bRawBufferAvailable);
	printf("use raw framebuffer: %d\n", gapi->useVga);
	printf("video surface bpp: %d\n", video->format->BitsPerPixel);
	printf("video surface width: %d\n", video->w);
	printf("video surface height: %d\n", video->h);
	printf("mouse/arrows transformation angle: %d\n", gapi->coordinateTransform);
#endif


	
	allScreen.x = allScreen.y = 0;
	allScreen.w = video->w - 1;
	allScreen.h = video->h - 1;
	GAPI_UpdateRects(this, 1, &allScreen);

	
	return(video);
}

static int GAPI_AllocHWSurface(_THIS, SDL_Surface *surface)
{
	return(-1);
}
static void GAPI_FreeHWSurface(_THIS, SDL_Surface *surface)
{
	return;
}

static int GAPI_LockHWSurface(_THIS, SDL_Surface *surface)
{
	return(0);
}

static void GAPI_UnlockHWSurface(_THIS, SDL_Surface *surface)
{
	return;
}

static int updateLine8to8(_THIS, unsigned char *srcPointer, unsigned char *destPointer, int width, int height, int lines)
{
	if( gapi->dstPixelStep == 1) 
	{
		SDL_memcpy(destPointer, srcPointer, width);
		return 1;
	} else
	{
		
		int step = gapi->dstPixelStep;
		while(width--)
		{
			*destPointer = *srcPointer++;
			destPointer += step;
		}
	}
	return 1;
}

static int updateLine16to16(_THIS, PIXEL *srcPointer, PIXEL *destPointer, int width, int height, int lines)
{
	PIXEL *line1, *line2;
	int step = gapi->dstPixelStep / 2;

	if( step == 1 ) 
	{
		SDL_memcpy(destPointer, srcPointer, width * sizeof(PIXEL));
		return 1;
	}
	else
	{
		if( (gapi->gapiOrientation != SDL_ORIENTATION_UP) &&
			(gapi->userOrientation == SDL_ORIENTATION_UP )) 
		{	
			
			if( ((((unsigned)destPointer & 3) != 0) && (gapi->gapiOrientation == SDL_ORIENTATION_LEFT)) 
				|| ((((unsigned)destPointer & 3) == 0) && (gapi->gapiOrientation != SDL_ORIENTATION_LEFT))
				|| (lines == 1) ) 
			{
				while(width--)
				{
					*destPointer = *srcPointer++;
					destPointer += step;
				}
				return 1;
			}

			
			line1 = srcPointer;
			line2 = srcPointer + SDL_VideoSurface->pitch / 2;

			if( gapi->gapiOrientation == SDL_ORIENTATION_LEFT )
				while(width--) 
				{
					*(DWORD*)destPointer =(*line2++ << 16) | *line1++;
					destPointer += step;
				}
			else
			{
				destPointer += gapi->gxProperties.cbyPitch / 2;

				while(width--) 
				{
					*(DWORD*)destPointer =(*line1++ << 16) | *line2++;
					destPointer += step;
				}
			}
			return 2;
		} else
		{
			
			if( gapi->gapiOrientation == SDL_ORIENTATION_LEFT )
			{
				int w1;

				
				if( (((unsigned)destPointer & 3) == 0) && (width > 0)) 
				{
					*destPointer-- = *srcPointer++;
					width--;
				}

				destPointer--;

				w1 = width / 2;

				while(w1--)
				{
					DWORD p = *(DWORD*)srcPointer;
					*((DWORD*)destPointer) = (p << 16) | (p >> 16);
					destPointer -= 2;
					srcPointer += 2;
				}

				if( width & 1 ) 
				{
					destPointer++;
					*destPointer = *srcPointer;
				}

				return 1;
			}

			
			

			line1 = srcPointer;
			line2 = srcPointer + SDL_VideoSurface->pitch / 2;

			if( (((unsigned)destPointer & 3) != 0) || (lines == 1) ) 
			{
				while(width--)
				{
					*destPointer = *srcPointer++;
					destPointer += step;
				}
				return 1;
			}
			
			while(width--)
			{
				*(DWORD*)destPointer =(*line2++ << 16) | *line1++;
				destPointer -= gapi->gxProperties.cbyPitch / 2;
			}
			return 2;
		}
	}
}

#define REDMASK (31<<11)
#define GREENMASK (63<<5)
#define BLUEMASK (31)


static int updateLine16to4(_THIS, PIXEL *srcPointer, unsigned char *destPointer, int width, int height, int lines, int yNibble, int xNibble)
{
	PIXEL *line1, *line2;
	int step = gapi->dstPixelStep;

	if( gapi->userOrientation == SDL_ORIENTATION_UP )
	{
		if( yNibble ) 
		{
			while(width--)
			{
				PIXEL c1 = *srcPointer++;
				c1 = ((c1 & REDMASK) >> 11) + ((c1 & GREENMASK) >> 5) + (c1 & BLUEMASK);			
				*destPointer = (*destPointer & 0x0F) | ((~(c1 >> 3) << 4));
				destPointer += step;
			}
			return 1;
		}

		
		if( lines == 1 )
		{
			while(width--)
			{
				PIXEL c1 = *srcPointer++;
				c1 = ((c1 & REDMASK) >> 11) + ((c1 & GREENMASK) >> 5) + (c1 & BLUEMASK);			
				*destPointer = (*destPointer & 0xF0) | ((~(c1 >> 3) & 0xF));
				destPointer += step;
			}
			return 1;
		}

		line1 = srcPointer;
		line2 = srcPointer + SDL_VideoSurface->pitch / 2;

		while(width--)
		{
			PIXEL c1 = *line1++;
			PIXEL c2 = *line2++;
			c1 = ((c1 & REDMASK) >> 11) + ((c1 & GREENMASK) >> 5) + (c1 & BLUEMASK);
			c2 = ((c2 & REDMASK) >> 11) + ((c2 & GREENMASK) >> 5) + (c2 & BLUEMASK);
			*destPointer = ~((c1 >> 3) + ((c2 >> 3) << 4));
			destPointer += step;
		}
		return 2;
	} else
	{
		int w1;
		w1 = width / 2;

		if( xNibble )
		{
			
			PIXEL c1 = *srcPointer++;
			c1 = ((c1 & REDMASK) >> 11) + ((c1 & GREENMASK) >> 5) + (c1 & BLUEMASK);			
			*destPointer = (*destPointer & 0xF0) | ((~(c1 >> 3) & 0xF));
			destPointer++;
		}

		while(w1--)
		{
			PIXEL c1 = *srcPointer;
			PIXEL c2 = *(srcPointer + 1);
			c1 = ((c1 & REDMASK) >> 11) + ((c1 & GREENMASK) >> 5) + (c1 & BLUEMASK);
			c2 = ((c2 & REDMASK) >> 11) + ((c2 & GREENMASK) >> 5) + (c2 & BLUEMASK);
			*destPointer++ = ~((c2 >> 3) + ((c1 >> 3) << 4));
			srcPointer += 2;
		}

		
		if( (width & 1) && !xNibble )
		{
			PIXEL c1 = *srcPointer;
			c1 = ((c1 & REDMASK) >> 11) + ((c1 & GREENMASK) >> 5) + (c1 & BLUEMASK);			
			*destPointer = (*destPointer & 0x0F) | ((~(c1 >> 3) << 4));
		}

		return 1;
	}
}

static void GAPI_UpdateRectsMono(_THIS, int numrects, SDL_Rect *rects)
{
	int i, height;
	int linesProcessed;
	int xNibble, yNibble;

	for (i=0; i<numrects; i++)
	{
		unsigned char *destPointer;
		unsigned char *srcPointer;

		if( gapi->userOrientation == SDL_ORIENTATION_UP )
			destPointer = (unsigned char*) gapi->videoMem + gapi->startOffset - rects[i].y * gapi->gxProperties.cBPP / 8 + rects[i].x * gapi->dstPixelStep;
		else
			destPointer = (unsigned char*) gapi->videoMem + gapi->startOffset + rects[i].x * gapi->gxProperties.cBPP / 8 + rects[i].y * gapi->dstLineStep;

		srcPointer = ((unsigned char*) SDL_VideoSurface->pixels) + rects[i].y * SDL_VideoSurface->pitch + rects[i].x * 2;
		yNibble = rects[i].y & 1; 
		xNibble = rects[i].x & 1;
		height = rects[i].h;
		while (height > 0)
		{
			switch(gapi->gxProperties.cBPP)
			{
			case 2: 
			case 4:
					linesProcessed = updateLine16to4(this, (PIXEL*) srcPointer, destPointer, rects[i].w, rects[i].h, height, yNibble, xNibble);
					yNibble = 0;
			}
			height -= linesProcessed;
			if( gapi->userOrientation == SDL_ORIENTATION_UP )
				destPointer--; 
			else destPointer += gapi->dstLineStep;
			srcPointer += SDL_VideoSurface->pitch * linesProcessed; 
		}
	}
}

static void GAPI_UpdateRectsColor(_THIS, int numrects, SDL_Rect *rects)
{
	int i, height;
	int bytesPerPixel = (gapi->gxProperties.cBPP + 1) / 8;
	int linesProcessed;
	for (i=0; i<numrects; i++) {
		unsigned char *destPointer = (unsigned char*) gapi->videoMem + gapi->startOffset + rects[i].y * gapi->dstLineStep + rects[i].x * gapi->dstPixelStep;
		unsigned char *srcPointer = ((unsigned char*) SDL_VideoSurface->pixels) + rects[i].y * SDL_VideoSurface->pitch + rects[i].x * bytesPerPixel;
		height = rects[i].h;

		linesProcessed = height;

		while (height > 0) {
			switch(bytesPerPixel)
			{
			case 1:
				linesProcessed = updateLine8to8(this, srcPointer, (unsigned char *) destPointer, rects[i].w, rects[i].h, height);
				break;
			case 2:
#pragma warning(disable: 4133)
				linesProcessed = updateLine16to16(this, (PIXEL*) srcPointer, destPointer, rects[i].w, rects[i].h, height);
				break;
			}
			height -= linesProcessed;
			destPointer += gapi->dstLineStep * linesProcessed;
			srcPointer += SDL_VideoSurface->pitch * linesProcessed; 
		}
	}
}


static void GAPI_UpdateRects(_THIS, int numrects, SDL_Rect *rects)
{
	
	if( gapi->suspended ) return;

	if( gapi->needUpdate )
		gapi->videoMem = gapi->gxFunc.GXBeginDraw(); 

	if( gapi->gxProperties.cBPP < 8 )
		GAPI_UpdateRectsMono(this, numrects, rects);
	else
		GAPI_UpdateRectsColor(this, numrects, rects);

	if( gapi->needUpdate )
		gapi->gxFunc.GXEndDraw();
}

void GAPI_VideoQuit(_THIS)
{
	int i, j;
	
	if ( SDL_Window ) 
	{
	    if ((g_hGapiLib != 0) && this && gapi && gapi->gxFunc.GXCloseDisplay && !gapi->useVga)
			gapi->gxFunc.GXCloseDisplay(); 

		if (this->screen->pixels != NULL)
		{
			SDL_free(this->screen->pixels);
			this->screen->pixels = NULL;
		}
		if ( screen_icn ) {
			DestroyIcon(screen_icn);
			screen_icn = NULL;
		}

		DIB_DestroyWindow(this);
		SDL_UnregisterApp();

		SDL_Window = NULL;
#if defined(_WIN32_WCE)

		if( aygshell ) 
		{
			FreeLibrary(aygshell);
			aygshell = NULL;
		}
#endif

	
	for ( i=0; i<NUM_MODELISTS; ++i ) {
		if ( gapi->SDL_modelist[i] != NULL ) {
			for ( j=0; gapi->SDL_modelist[i][j]; ++j )
				SDL_free(gapi->SDL_modelist[i][j]);
			SDL_free(gapi->SDL_modelist[i]);
			gapi->SDL_modelist[i] = NULL;
		}
	}

	}

}

static void GAPI_Activate(_THIS, BOOL active, BOOL minimized)
{
	
}

static void GAPI_RealizePalette(_THIS)
{
	OutputDebugString(TEXT("GAPI_RealizePalette NOT IMPLEMENTED !\r\n"));
}

static void GAPI_PaletteChanged(_THIS, HWND window)
{
	OutputDebugString(TEXT("GAPI_PaletteChanged NOT IMPLEMENTED !\r\n"));
}

static void GAPI_WinPAINT(_THIS, HDC hdc)
{
	

	int bpp = 16; 
	HGDIOBJ prevObject;
	unsigned short *bitmapData;
	HBITMAP hb;
	HDC srcDC;

    
    BYTE buffer[sizeof(BITMAPINFOHEADER) + 3 * sizeof(RGBQUAD)] = {0};
    BITMAPINFO*       pBMI    = (BITMAPINFO*)buffer;
    BITMAPINFOHEADER* pHeader = &pBMI->bmiHeader;
    DWORD*            pColors = (DWORD*)&pBMI->bmiColors;   

	
	if( gapi->gxProperties.cBPP == 8 ) return;

    
    pHeader->biSize            = sizeof(BITMAPINFOHEADER);
    pHeader->biWidth           = gapi->w;
    pHeader->biHeight          = -gapi->h;
    pHeader->biPlanes          = 1;
    pHeader->biBitCount        = bpp;
    pHeader->biCompression     = BI_RGB;
    pHeader->biSizeImage       = (gapi->w * gapi->h * bpp) / 8;
	
    
	if( bpp == 16 )
	{
		pColors[0] = REDMASK;
		pColors[1] = GREENMASK;
		pColors[2] = BLUEMASK;
		pHeader->biCompression = BI_BITFIELDS;
	}
    
    hb =  CreateDIBSection( 0, pBMI, DIB_RGB_COLORS, (void**)&bitmapData, 0, 0 );

	
	
	memcpy(bitmapData, gapi->buffer, pHeader->biSizeImage);
	srcDC = CreateCompatibleDC(hdc);
	prevObject = SelectObject(srcDC, hb);

	BitBlt(hdc, 0, 0, gapi->w, gapi->h, srcDC, 0, 0, SRCCOPY);

	SelectObject(srcDC, prevObject);
	DeleteObject(hb);
	DeleteDC(srcDC);
}

int GAPI_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors) 
{
	GAPI_CreatePalette(ncolors, colors);
	return 1;
}

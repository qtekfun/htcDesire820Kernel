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

#ifndef _SDL_gapivideo_h
#define _SDL_gapivideo_h

#include "SDL_mouse.h"
#include "SDL_mutex.h"
#include "../SDL_sysvideo.h"
#include "../windib/SDL_gapidibvideo.h"


struct GXDisplayProperties {
    DWORD cxWidth;
    DWORD cyHeight;         
    long cbxPitch;          
    long cbyPitch;          
    long cBPP;              
    DWORD ffFormat;         
};

struct GXKeyList {
    short vkUp;             
    POINT ptUp;             
    short vkDown;
    POINT ptDown;
    short vkLeft;
    POINT ptLeft;
    short vkRight;
    POINT ptRight;
    short vkA;
    POINT ptA;
    short vkB;
    POINT ptB;
    short vkC;
    POINT ptC;
    short vkStart;
    POINT ptStart;
};

typedef int   (*PFNGXOpenDisplay)(HWND hWnd, DWORD dwFlags);
typedef int   (*PFNGXCloseDisplay)();
typedef void* (*PFNGXBeginDraw)();
typedef int   (*PFNGXEndDraw)();
typedef int   (*PFNGXOpenInput)();
typedef int   (*PFNGXCloseInput)();
typedef struct GXDisplayProperties (*PFNGXGetDisplayProperties)();
typedef struct GXKeyList (*PFNGXGetDefaultKeys)(int iOptions);
typedef int   (*PFNGXSuspend)();
typedef int   (*PFNGXResume)();
typedef int   (*PFNGXSetViewport)( DWORD dwTop, DWORD dwHeight, DWORD dwReserved1, DWORD dwReserved2 );
typedef BOOL  (*PFNGXIsDisplayDRAMBuffer)();

struct GapiFunc
{
    PFNGXOpenDisplay          GXOpenDisplay;
    PFNGXCloseDisplay         GXCloseDisplay;
    PFNGXBeginDraw            GXBeginDraw;
    PFNGXEndDraw              GXEndDraw;
    PFNGXOpenInput            GXOpenInput;
    PFNGXCloseInput           GXCloseInput;
    PFNGXGetDisplayProperties GXGetDisplayProperties;
    PFNGXGetDefaultKeys       GXGetDefaultKeys;
    PFNGXSuspend              GXSuspend;
    PFNGXResume               GXResume;
    PFNGXSetViewport          GXSetViewport;
    PFNGXIsDisplayDRAMBuffer  GXIsDisplayDRAMBuffer;
};

#define kfLandscape	0x8			
#define kfPalette	0x10		
#define kfDirect	0x20		
#define kfDirect555	0x40		
#define kfDirect565	0x80		
#define kfDirect888	0x100		
#define kfDirect444	0x200		
#define kfDirectInverted 0x400

#define GX_FULLSCREEN	0x01		
#define GX_NORMALKEYS   0x02
#define GX_LANDSCAPEKEYS        0x03


typedef enum {
	GAPI_NONE = 0,
	GAPI_DIRECT_565,
	GAPI_DIRECT_555,
	GAPI_MONO,
	GAPI_PALETTE
} GAPIVideoMode; 

typedef unsigned short PIXEL;

struct GapiInfo {
	int coordinateTransform; 
	char hiresFix; 
	int invert; //TODO this is only written but never read, so it should be removed

#define NUM_MODELISTS	4		
	int SDL_nummodes[NUM_MODELISTS];
	SDL_Rect **SDL_modelist[NUM_MODELISTS];
	

	
	
	SDL_ScreenOrientation userOrientation;
	SDL_ScreenOrientation systemOrientation;
	int useGXOpenDisplay; 
	int alreadyGXOpened;
	int w, h;
	
	
	SDL_ScreenOrientation gapiOrientation;

	void *buffer; 
	PIXEL *videoMem;
	BOOL needUpdate;
	struct GXKeyList keyList;
	struct GapiFunc gxFunc;
	struct GXDisplayProperties gxProperties;
	GAPIVideoMode videoMode;
	int colorscale;
	int dstLineStep;  
	int dstPixelStep; 
	int startOffset; 
	int useVga;
	int suspended; 
};



#endif 

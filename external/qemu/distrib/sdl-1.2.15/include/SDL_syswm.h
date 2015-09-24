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


#ifndef _SDL_syswm_h
#define _SDL_syswm_h

#include "SDL_stdinc.h"
#include "SDL_error.h"
#include "SDL_version.h"

#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifdef SDL_PROTOTYPES_ONLY
struct SDL_SysWMinfo;
typedef struct SDL_SysWMinfo SDL_SysWMinfo;
#else

#if defined(SDL_VIDEO_DRIVER_X11)
#if defined(__APPLE__) && defined(__MACH__)
#define Cursor X11Cursor
#endif

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#if defined(__APPLE__) && defined(__MACH__)
#undef Cursor
#endif

typedef enum {
	SDL_SYSWM_X11
} SDL_SYSWM_TYPE;

struct SDL_SysWMmsg {
	SDL_version version;
	SDL_SYSWM_TYPE subsystem;
	union {
	    XEvent xevent;
	} event;
};

typedef struct SDL_SysWMinfo {
	SDL_version version;
	SDL_SYSWM_TYPE subsystem;
	union {
	    struct {
	    	Display *display;	
	    	Window window;		
                
		void (*lock_func)(void);
		void (*unlock_func)(void);
                

		
                
	    	Window fswindow;	
	    	Window wmwindow;	
                

		
                
		Display *gfxdisplay;	
                
	    } x11;
	} info;
} SDL_SysWMinfo;

#elif defined(SDL_VIDEO_DRIVER_NANOX)
#include <microwin/nano-X.h>

struct SDL_SysWMmsg {
	SDL_version version;
	int data;
};

typedef struct SDL_SysWMinfo {
	SDL_version version ;
	GR_WINDOW_ID window ;	
} SDL_SysWMinfo;

#elif defined(SDL_VIDEO_DRIVER_WINDIB) || defined(SDL_VIDEO_DRIVER_DDRAW) || defined(SDL_VIDEO_DRIVER_GAPI)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct SDL_SysWMmsg {
	SDL_version version;
	HWND hwnd;			
	UINT msg;			
	WPARAM wParam;			
	LPARAM lParam;			
};

typedef struct SDL_SysWMinfo {
	SDL_version version;
	HWND window;			
	HGLRC hglrc;			
} SDL_SysWMinfo;

#elif defined(SDL_VIDEO_DRIVER_RISCOS)

struct SDL_SysWMmsg {
	SDL_version version;
	int eventCode;		
	int pollBlock[64];
};

typedef struct SDL_SysWMinfo {
	SDL_version version;
	int wimpVersion;    
	int taskHandle;     
	int window;		
} SDL_SysWMinfo;

#elif defined(SDL_VIDEO_DRIVER_PHOTON)
#include <sys/neutrino.h>
#include <Ph.h>

struct SDL_SysWMmsg {
	SDL_version version;
	int data;
};

typedef struct SDL_SysWMinfo {
	SDL_version version;
	int data;
} SDL_SysWMinfo;

#elif defined(SDL_VIDEO_DRIVER_QUARTZ)

struct SDL_SysWMmsg {
	SDL_version version;
	int data;
};

typedef struct SDL_SysWMinfo {
	SDL_version version;
	void* nsWindowPtr;
} SDL_SysWMinfo;

#else

struct SDL_SysWMmsg {
	SDL_version version;
	int data;
};

typedef struct SDL_SysWMinfo {
	SDL_version version;
	int data;
} SDL_SysWMinfo;

#endif 

#endif 

extern DECLSPEC int SDLCALL SDL_GetWMInfo(SDL_SysWMinfo *info);


#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif 

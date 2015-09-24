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

#ifndef _SDL_lowvideo_h
#define _SDL_lowvideo_h

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifndef SetClassLongPtr
#define SetClassLongPtr	SetClassLong
#endif
#ifndef GetWindowLongPtr
#define GetWindowLongPtr	GetWindowLong
#endif
#ifndef SetWindowLongPtr
#define SetWindowLongPtr	SetWindowLong
#endif
#ifndef GWLP_WNDPROC
#define GWLP_WNDPROC	GWL_WNDPROC
#endif
#ifndef GWLP_HINSTANCE
#define GWLP_HINSTANCE GWL_HINSTANCE
#endif
#ifndef GCLP_HICON
#define GCLP_HICON GCL_HICON
#endif

#include "../SDL_sysvideo.h"

#define _THIS	SDL_VideoDevice *this

#define FULLSCREEN() \
	((SDL_VideoSurface->flags & SDL_FULLSCREEN) == SDL_FULLSCREEN)

#define WINDIB_FULLSCREEN()						\
(									\
	SDL_VideoSurface &&						\
	FULLSCREEN() && \
	(((SDL_VideoSurface->flags & SDL_OPENGL   ) == SDL_OPENGL    ) || \
	((SDL_strcmp(this->name, "windib") == 0) || \
	 (SDL_strcmp(this->name, "gapi") == 0))) \
)
#define DDRAW_FULLSCREEN() 						\
(									\
	SDL_VideoSurface &&						\
	FULLSCREEN() && \
	((SDL_VideoSurface->flags & SDL_OPENGL    ) != SDL_OPENGL    ) && \
	(SDL_strcmp(this->name, "directx") == 0)				\
)

#define DINPUT_FULLSCREEN() 						\
(									\
	FULLSCREEN() && \
	(strcmp(this->name, "directx") == 0)				\
)

#define DINPUT() (strcmp(this->name, "directx") == 0)

#ifdef _WIN32_WCE
extern LPWSTR SDL_Appname;
#else
extern LPSTR SDL_Appname;
#endif
extern HINSTANCE SDL_Instance;
extern HWND SDL_Window;
extern BOOL SDL_windowid;

extern void WIN_FlushMessageQueue();

extern void (*WIN_Activate)(_THIS, BOOL active, BOOL minimized);

extern void (*WIN_RealizePalette)(_THIS);

extern void (*WIN_PaletteChanged)(_THIS, HWND window);

extern void (*WIN_WinPAINT)(_THIS, HDC hdc);

extern LONG (*HandleMessage)(_THIS, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

extern HCURSOR SDL_hcursor;

extern RECT SDL_bounds;

extern int SDL_windowX;
extern int SDL_windowY;

extern int SDL_resizing;

extern int mouse_relative;

#ifndef NO_CHANGEDISPLAYSETTINGS
extern DEVMODE SDL_desktop_mode;
extern DEVMODE SDL_fullscreen_mode;
#endif

extern WORD *gamma_saved;

extern void DX5_SoundFocus(HWND window);

LRESULT CALLBACK WinMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#ifdef _WIN64
#define SDL_ToUnicode  ToUnicode
#else
typedef int (WINAPI *ToUnicodeFN)(UINT, UINT, const BYTE *, LPWSTR, int, UINT);

extern ToUnicodeFN SDL_ToUnicode;
#endif

#endif 

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


#ifndef _SDL_mouse_h
#define _SDL_mouse_h

#include "SDL_stdinc.h"
#include "SDL_error.h"
#include "SDL_video.h"

#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct WMcursor WMcursor;	
typedef struct SDL_Cursor {
	SDL_Rect area;			
	Sint16 hot_x, hot_y;		
	Uint8 *data;			
	Uint8 *mask;			
	Uint8 *save[2];			
	WMcursor *wm_cursor;		
} SDL_Cursor;

extern DECLSPEC Uint8 SDLCALL SDL_GetMouseState(int *x, int *y);

extern DECLSPEC Uint8 SDLCALL SDL_GetRelativeMouseState(int *x, int *y);

extern DECLSPEC void SDLCALL SDL_WarpMouse(Uint16 x, Uint16 y);

extern DECLSPEC SDL_Cursor * SDLCALL SDL_CreateCursor
		(Uint8 *data, Uint8 *mask, int w, int h, int hot_x, int hot_y);

extern DECLSPEC void SDLCALL SDL_SetCursor(SDL_Cursor *cursor);

extern DECLSPEC SDL_Cursor * SDLCALL SDL_GetCursor(void);

extern DECLSPEC void SDLCALL SDL_FreeCursor(SDL_Cursor *cursor);

extern DECLSPEC int SDLCALL SDL_ShowCursor(int toggle);

#define SDL_BUTTON(X)		(1 << ((X)-1))
#define SDL_BUTTON_LEFT		1
#define SDL_BUTTON_MIDDLE	2
#define SDL_BUTTON_RIGHT	3
#define SDL_BUTTON_WHEELUP	4
#define SDL_BUTTON_WHEELDOWN	5
#define SDL_BUTTON_X1		6
#define SDL_BUTTON_X2		7
#define SDL_BUTTON_LMASK	SDL_BUTTON(SDL_BUTTON_LEFT)
#define SDL_BUTTON_MMASK	SDL_BUTTON(SDL_BUTTON_MIDDLE)
#define SDL_BUTTON_RMASK	SDL_BUTTON(SDL_BUTTON_RIGHT)
#define SDL_BUTTON_X1MASK	SDL_BUTTON(SDL_BUTTON_X1)
#define SDL_BUTTON_X2MASK	SDL_BUTTON(SDL_BUTTON_X2)

#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif 

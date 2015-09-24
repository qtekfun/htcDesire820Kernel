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


#ifndef _SDL_keyboard_h
#define _SDL_keyboard_h

#include "SDL_stdinc.h"
#include "SDL_error.h"
#include "SDL_keysym.h"

#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct SDL_keysym {
	Uint8 scancode;			
	SDLKey sym;			
	SDLMod mod;			
	Uint16 unicode;			
} SDL_keysym;

#define SDL_ALL_HOTKEYS		0xFFFFFFFF

extern DECLSPEC int SDLCALL SDL_EnableUNICODE(int enable);

#define SDL_DEFAULT_REPEAT_DELAY	500
#define SDL_DEFAULT_REPEAT_INTERVAL	30
extern DECLSPEC int SDLCALL SDL_EnableKeyRepeat(int delay, int interval);
extern DECLSPEC void SDLCALL SDL_GetKeyRepeat(int *delay, int *interval);

extern DECLSPEC Uint8 * SDLCALL SDL_GetKeyState(int *numkeys);

extern DECLSPEC SDLMod SDLCALL SDL_GetModState(void);

extern DECLSPEC void SDLCALL SDL_SetModState(SDLMod modstate);

extern DECLSPEC char * SDLCALL SDL_GetKeyName(SDLKey key);


#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif 

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


#ifndef _SDL_joystick_h
#define _SDL_joystick_h

#include "SDL_stdinc.h"
#include "SDL_error.h"

#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif


struct _SDL_Joystick;
typedef struct _SDL_Joystick SDL_Joystick;

extern DECLSPEC int SDLCALL SDL_NumJoysticks(void);

extern DECLSPEC const char * SDLCALL SDL_JoystickName(int device_index);

extern DECLSPEC SDL_Joystick * SDLCALL SDL_JoystickOpen(int device_index);

extern DECLSPEC int SDLCALL SDL_JoystickOpened(int device_index);

extern DECLSPEC int SDLCALL SDL_JoystickIndex(SDL_Joystick *joystick);

extern DECLSPEC int SDLCALL SDL_JoystickNumAxes(SDL_Joystick *joystick);

extern DECLSPEC int SDLCALL SDL_JoystickNumBalls(SDL_Joystick *joystick);

extern DECLSPEC int SDLCALL SDL_JoystickNumHats(SDL_Joystick *joystick);

extern DECLSPEC int SDLCALL SDL_JoystickNumButtons(SDL_Joystick *joystick);

extern DECLSPEC void SDLCALL SDL_JoystickUpdate(void);

extern DECLSPEC int SDLCALL SDL_JoystickEventState(int state);

extern DECLSPEC Sint16 SDLCALL SDL_JoystickGetAxis(SDL_Joystick *joystick, int axis);

#define SDL_HAT_CENTERED	0x00
#define SDL_HAT_UP		0x01
#define SDL_HAT_RIGHT		0x02
#define SDL_HAT_DOWN		0x04
#define SDL_HAT_LEFT		0x08
#define SDL_HAT_RIGHTUP		(SDL_HAT_RIGHT|SDL_HAT_UP)
#define SDL_HAT_RIGHTDOWN	(SDL_HAT_RIGHT|SDL_HAT_DOWN)
#define SDL_HAT_LEFTUP		(SDL_HAT_LEFT|SDL_HAT_UP)
#define SDL_HAT_LEFTDOWN	(SDL_HAT_LEFT|SDL_HAT_DOWN)

extern DECLSPEC Uint8 SDLCALL SDL_JoystickGetHat(SDL_Joystick *joystick, int hat);

extern DECLSPEC int SDLCALL SDL_JoystickGetBall(SDL_Joystick *joystick, int ball, int *dx, int *dy);

extern DECLSPEC Uint8 SDLCALL SDL_JoystickGetButton(SDL_Joystick *joystick, int button);

extern DECLSPEC void SDLCALL SDL_JoystickClose(SDL_Joystick *joystick);


#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif 

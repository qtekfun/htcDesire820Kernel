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

#if defined(SDL_JOYSTICK_DUMMY) || defined(SDL_JOYSTICK_DISABLED)


#include "SDL_joystick.h"
#include "../SDL_sysjoystick.h"
#include "../SDL_joystick_c.h"

int SDL_SYS_JoystickInit(void)
{
	SDL_numjoysticks = 0;
	return(0);
}

const char *SDL_SYS_JoystickName(int index)
{
	SDL_SetError("Logic error: No joysticks available");
	return(NULL);
}

int SDL_SYS_JoystickOpen(SDL_Joystick *joystick)
{
	SDL_SetError("Logic error: No joysticks available");
	return(-1);
}

void SDL_SYS_JoystickUpdate(SDL_Joystick *joystick)
{
	return;
}

void SDL_SYS_JoystickClose(SDL_Joystick *joystick)
{
	return;
}

void SDL_SYS_JoystickQuit(void)
{
	return;
}

#endif 

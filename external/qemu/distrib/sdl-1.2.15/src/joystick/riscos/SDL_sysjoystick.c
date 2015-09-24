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

#ifdef SDL_JOYSTICK_RISCOS



#include "SDL_events.h"
#include "SDL_joystick.h"
#include "../SDL_sysjoystick.h"
#include "../SDL_joystick_c.h"

#include "kernel.h"

#define JOYSTICK_READ 0x43F40

struct joystick_hwdata 
{
	int joystate;
};


int SDL_SYS_JoystickInit(void)
{
	_kernel_swi_regs regs;

	 
	regs.r[0] = 0;
	if (_kernel_swi(JOYSTICK_READ, &regs, &regs) == NULL)
	{
		
		return 1;
	}
	

	return(0);
}

const char *SDL_SYS_JoystickName(int index)
{
	if (index == 0)
	{
		return "RISC OS Joystick 0";
	}

	SDL_SetError("No joystick available with that index");
	return(NULL);
}

int SDL_SYS_JoystickOpen(SDL_Joystick *joystick)
{
	_kernel_swi_regs regs;

	if(!(joystick->hwdata=SDL_malloc(sizeof(struct joystick_hwdata))))
		return -1;

	regs.r[0] = joystick->index;

	
	joystick->nbuttons=4;

	joystick->nhats=0;
	joystick->nballs=0;
	joystick->naxes=2;
	joystick->hwdata->joystate=0;

	return 0;

}

void SDL_SYS_JoystickUpdate(SDL_Joystick *joystick)
{
	_kernel_swi_regs regs;
	regs.r[0] = joystick->index;

	if (_kernel_swi(JOYSTICK_READ, &regs, &regs) == NULL)
	{
		int newstate = regs.r[0];
		int oldstate = joystick->hwdata->joystate;
		if (newstate != oldstate)
		{
			if ((newstate & 0xFF) != (oldstate & 0xFF))
			{
				int y = regs.r[0] & 0xFF;
				
				if (y >= 128) y -= 256;
				SDL_PrivateJoystickAxis(joystick,1,-y * 256); 
			}
			if ((newstate & 0xFF00) != (oldstate & 0xFF00))
			{
				int x = (regs.r[0] & 0xFF00) >> 8;
				if (x >= 128) x -= 256;
				SDL_PrivateJoystickAxis(joystick,0,x * 256);
			}

			if ((newstate & 0xFF0000) != (oldstate & 0xFF0000))
			{
				int buttons = (regs.r[0] & 0xFF0000) >> 16;
				int oldbuttons = (oldstate & 0xFF0000) >> 16;
				int i;
				for (i = 0; i < joystick->nbuttons; i++)
				{
					if ((buttons & (1<<i)) != (oldbuttons & (1<<i)))
					{
						if (buttons & (1<<i)) SDL_PrivateJoystickButton(joystick,i,SDL_PRESSED);
						else SDL_PrivateJoystickButton(joystick,i,SDL_RELEASED);
					}
				}
			}
			joystick->hwdata->joystate = newstate;
		}		
	}

	return;
}

void SDL_SYS_JoystickClose(SDL_Joystick *joystick)
{
	if(joystick->hwdata)
		SDL_free(joystick->hwdata);
	return;
}

void SDL_SYS_JoystickQuit(void)
{
	SDL_numjoysticks=0;

	return;
}

#endif 

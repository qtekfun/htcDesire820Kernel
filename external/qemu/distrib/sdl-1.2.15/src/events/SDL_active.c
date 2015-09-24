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


#include "SDL_events.h"
#include "SDL_events_c.h"


static Uint8 SDL_appstate = 0;

int SDL_AppActiveInit(void)
{
	
	SDL_appstate = (SDL_APPACTIVE|SDL_APPINPUTFOCUS|SDL_APPMOUSEFOCUS);

	
	return(0);
}
void SDL_AppActiveQuit(void)
{
}

Uint8 SDL_GetAppState(void)
{
	return(SDL_appstate);
}

int SDL_PrivateAppActive(Uint8 gain, Uint8 state)
{
	int posted;
	Uint8 new_state;

	
	if ( gain ) {
		new_state = (SDL_appstate | state);
	} else {
		new_state = (SDL_appstate & ~state);
	}

	
	if ( new_state == SDL_appstate ) {
		return(0);
	}

	
	SDL_appstate = new_state;

	
	posted = 0;
	if ( SDL_ProcessEvents[SDL_ACTIVEEVENT] == SDL_ENABLE ) {
		SDL_Event event;
		SDL_memset(&event, 0, sizeof(event));
		event.type = SDL_ACTIVEEVENT;
		event.active.gain = gain;
		event.active.state = state;
		if ( (SDL_EventOK == NULL) || (*SDL_EventOK)(&event) ) {
			posted = 1;
			SDL_PushEvent(&event);
		}
	}

	
	if ( (state & SDL_APPINPUTFOCUS) && !gain ) {
		SDL_ResetKeyboard();
	}
	
	if ( (state & SDL_APPACTIVE) && !gain ) {
		SDL_ResetMouse();
	}
	return(posted);
}

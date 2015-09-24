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


#include <stdio.h>
#include <X11/Xlib.h>
#include "../Xext/extensions/xf86dga.h"

#include "SDL_timer.h"
#include "../SDL_sysvideo.h"
#include "../../events/SDL_events_c.h"
#include "SDL_dgavideo.h"
#include "SDL_dgaevents_c.h"

#include "../x11/SDL_x11dyn.h"

extern int X11_Pending(Display *display);
extern void X11_InitKeymap(void);
extern SDLKey X11_TranslateKeycode(Display *display, KeyCode kc);

static int DGA_DispatchEvent(_THIS)
{
	int posted;
	SDL_NAME(XDGAEvent) xevent;

	XNextEvent(DGA_Display, (XEvent *)&xevent);

	posted = 0;
	xevent.type -= DGA_event_base;
	switch (xevent.type) {

	    
	    case MotionNotify: {
		if ( SDL_VideoSurface ) {
			posted = SDL_PrivateMouseMotion(0, 1,
					xevent.xmotion.dx, xevent.xmotion.dy);
		}
	    }
	    break;

	    
	    case ButtonPress: {
		posted = SDL_PrivateMouseButton(SDL_PRESSED, 
					xevent.xbutton.button, 0, 0);
	    }
	    break;

	    
	    case ButtonRelease: {
		posted = SDL_PrivateMouseButton(SDL_RELEASED, 
					xevent.xbutton.button, 0, 0);
	    }
	    break;

	    
	    case KeyPress: {
		SDL_keysym keysym;
		KeyCode keycode;
		XKeyEvent xkey;

		SDL_NAME(XDGAKeyEventToXKeyEvent)(&xevent.xkey, &xkey);
		keycode = xkey.keycode;
#ifdef DEBUG_XEVENTS
printf("KeyPress (X11 keycode = 0x%X)\n", xkey.keycode);
#endif
		
		keysym.scancode = keycode;
		keysym.sym = X11_TranslateKeycode(DGA_Display, keycode);
		keysym.mod = KMOD_NONE;
		keysym.unicode = 0;

		
		if ( SDL_TranslateUNICODE ) {
			static XComposeStatus state;
			char keybuf[32];

			if ( XLookupString(&xkey, keybuf, sizeof(keybuf), NULL, &state) ) {
				keysym.unicode = (Uint8)keybuf[0];
			}
		}
		posted = SDL_PrivateKeyboard(SDL_PRESSED, &keysym);
	    }
	    break;

	    
	    case KeyRelease: {
		SDL_keysym keysym;
		KeyCode keycode;
		XKeyEvent xkey;

		SDL_NAME(XDGAKeyEventToXKeyEvent)(&xevent.xkey, &xkey);
		keycode = xkey.keycode;
#ifdef DEBUG_XEVENTS
printf("KeyRelease (X11 keycode = 0x%X)\n", xkey.keycode);
#endif
		
		keysym.scancode = keycode;
		keysym.sym = X11_TranslateKeycode(DGA_Display, keycode);
		keysym.mod = KMOD_NONE;
		keysym.unicode = 0;
		posted = SDL_PrivateKeyboard(SDL_RELEASED, &keysym);
	    }
	    break;
	}
	return(posted);
}

void DGA_PumpEvents(_THIS)
{
	
	LOCK_DISPLAY();

	
	if (!allow_screensaver) {
		static Uint32 screensaverTicks;
		Uint32 nowTicks = SDL_GetTicks();
		if ((nowTicks - screensaverTicks) > 5000) {
			XResetScreenSaver(DGA_Display);
			screensaverTicks = nowTicks;
		}
	}

	while ( X11_Pending(DGA_Display) ) {
		DGA_DispatchEvent(this);
	}

	UNLOCK_DISPLAY();
}

void DGA_InitOSKeymap(_THIS)
{
	X11_InitKeymap();
}


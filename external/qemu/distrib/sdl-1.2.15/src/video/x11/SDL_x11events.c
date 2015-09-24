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


#include <setjmp.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#ifdef __SVR4
#include <X11/Sunkeysym.h>
#endif
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

#include "SDL_timer.h"
#include "SDL_syswm.h"
#include "../SDL_sysvideo.h"
#include "../../events/SDL_sysevents.h"
#include "../../events/SDL_events_c.h"
#include "SDL_x11video.h"
#include "SDL_x11dga_c.h"
#include "SDL_x11modes_c.h"
#include "SDL_x11image_c.h"
#include "SDL_x11gamma_c.h"
#include "SDL_x11wm_c.h"
#include "SDL_x11mouse_c.h"
#include "SDL_x11events_c.h"



static SDLKey ODD_keymap[256];
static SDLKey MISC_keymap[256];
SDLKey X11_TranslateKeycode(Display *display, KeyCode kc);

int X11_PendingConfigureNotifyWidth = -1;
int X11_PendingConfigureNotifyHeight = -1;

#ifdef X_HAVE_UTF8_STRING
Uint32 Utf8ToUcs4(const Uint8 *utf8)
{
	Uint32 c;
	int i = 1;
	int noOctets = 0;
	int firstOctetMask = 0;
	unsigned char firstOctet = utf8[0];
	if (firstOctet < 0x80) {
		noOctets = 1;
		firstOctetMask = 0x7F;  
	} else if ((firstOctet & 0xE0) 
	              == 0xC0 ) {  
		noOctets = 2;
		firstOctetMask = 0x1F;  
	} else if ((firstOctet & 0xF0) 
	              == 0xE0) {  
		noOctets = 3;
		firstOctetMask = 0x0F; 
	} else if ((firstOctet & 0xF8) 
	              == 0xF0) {  
		noOctets = 4;
		firstOctetMask = 0x07; 
	} else if ((firstOctet & 0xFC) 
	              == 0xF8) { 
		noOctets = 5;
		firstOctetMask = 0x03; 
	} else if ((firstOctet & 0xFE) 
	              == 0xFC) { 
		noOctets = 6;
		firstOctetMask = 0x01; 
	} else
		return 0;  
	
	c = firstOctet & firstOctetMask;
	
	
	for (i = 1; i < noOctets; i++) {
		
		if ((utf8[i] & 0xC0) 
		    != 0x80) 
			return 0;
		
		
		c <<= 6;
		
		c |= utf8[i] & 0x3F;
	}
	return c;
}


static int Utf8ToUtf16(const Uint8 *utf8, const int utf8_length, Uint16 *utf16, const int utf16_max_length) {

    
    Uint16 *p = utf16;
    Uint16 const *const max_ptr = utf16 + utf16_max_length;

    
    Uint8 const *const end_of_input = utf8 + utf8_length - 1;

    while (utf8 <= end_of_input) {
	Uint8 const c = *utf8;
	if (p >= max_ptr) {
	    
	    return -1;
	}
	if (c < 0x80) {
	    
	    *p++ = c;
	    utf8 += 1;
	} else if (c < 0xC0) {
	    
	    return -1;
	} else if (c < 0xE0) {
	    
	    if (end_of_input - utf8 < 1 || (((utf8[1] ^ 0x80)) & 0xC0)) {
		return -1;
	    }
	    *p++ = (Uint16)(0xCF80 + (c << 6) + utf8[1]);
	    utf8 += 2;
	} else if (c < 0xF0) {
	    
	    if (end_of_input - utf8 < 2 || (((utf8[1] ^ 0x80) | (utf8[2] ^ 0x80)) & 0xC0)) {
		return -1;
	    }
	    *p++ = (Uint16)(0xDF80 + (c << 12) + (utf8[1] << 6) + utf8[2]);
	    utf8 += 3;
	} else if (c < 0xF8) {
	    int plane;
	    
	    if (end_of_input - utf8 < 3 || (((utf8[1] ^ 0x80) | (utf8[2] ^0x80) | (utf8[3] ^ 0x80)) & 0xC0)) {
		return -1;
	    }
	    plane = (-0xC8 + (c << 2) + (utf8[1] >> 4));
	    if (plane == 0) {
		*p++ = (Uint16)(0xDF80 + (utf8[1] << 12) + (utf8[2] << 6) + utf8[3]);
	    } else if (plane <= 16) {
		
		if (p + 1 >= max_ptr) {
		    
		    return -1;
		}
		*p++ = (Uint16)(0xE5B8 + (c << 8) + (utf8[1] << 2) + (utf8[2] >> 4));
		*p++ = (Uint16)(0xDB80 + ((utf8[2] & 0x0F) << 6) + utf8[3]);
	    } else {
		
		return -1;
	    }
	    utf8 += 4;
	} else {
	    
	    return -1;
	}
    }
    return p - utf16;
}

#endif

static int X11_KeyRepeat(Display *display, XEvent *event)
{
	XEvent peekevent;
	int repeated;

	repeated = 0;
	if ( XPending(display) ) {
		XPeekEvent(display, &peekevent);
		if ( (peekevent.type == KeyPress) &&
		     (peekevent.xkey.keycode == event->xkey.keycode) &&
		     ((peekevent.xkey.time-event->xkey.time) < 2) ) {
			repeated = 1;
			XNextEvent(display, &peekevent);
		}
	}
	return(repeated);
}

#define MOUSE_FUDGE_FACTOR	8

static __inline__ int X11_WarpedMotion(_THIS, XEvent *xevent)
{
	int w, h, i;
	int deltax, deltay;
	int posted;

	w = SDL_VideoSurface->w;
	h = SDL_VideoSurface->h;
	deltax = xevent->xmotion.x - mouse_last.x;
	deltay = xevent->xmotion.y - mouse_last.y;
#ifdef DEBUG_MOTION
  printf("Warped mouse motion: %d,%d\n", deltax, deltay);
#endif
	mouse_last.x = xevent->xmotion.x;
	mouse_last.y = xevent->xmotion.y;
	posted = SDL_PrivateMouseMotion(0, 1, deltax, deltay);

	if ( (xevent->xmotion.x < MOUSE_FUDGE_FACTOR) ||
	     (xevent->xmotion.x > (w-MOUSE_FUDGE_FACTOR)) ||
	     (xevent->xmotion.y < MOUSE_FUDGE_FACTOR) ||
	     (xevent->xmotion.y > (h-MOUSE_FUDGE_FACTOR)) ) {
		
		while ( XCheckTypedEvent(SDL_Display, MotionNotify, xevent) ) {
			deltax = xevent->xmotion.x - mouse_last.x;
			deltay = xevent->xmotion.y - mouse_last.y;
#ifdef DEBUG_MOTION
  printf("Extra mouse motion: %d,%d\n", deltax, deltay);
#endif
			mouse_last.x = xevent->xmotion.x;
			mouse_last.y = xevent->xmotion.y;
			posted += SDL_PrivateMouseMotion(0, 1, deltax, deltay);
		}
		mouse_last.x = w/2;
		mouse_last.y = h/2;
		XWarpPointer(SDL_Display, None, SDL_Window, 0, 0, 0, 0,
					mouse_last.x, mouse_last.y);
		for ( i=0; i<10; ++i ) {
        		XMaskEvent(SDL_Display, PointerMotionMask, xevent);
			if ( (xevent->xmotion.x >
			          (mouse_last.x-MOUSE_FUDGE_FACTOR)) &&
			     (xevent->xmotion.x <
			          (mouse_last.x+MOUSE_FUDGE_FACTOR)) &&
			     (xevent->xmotion.y >
			          (mouse_last.y-MOUSE_FUDGE_FACTOR)) &&
			     (xevent->xmotion.y <
			          (mouse_last.y+MOUSE_FUDGE_FACTOR)) ) {
				break;
			}
#ifdef DEBUG_XEVENTS
  printf("Lost mouse motion: %d,%d\n", xevent->xmotion.x, xevent->xmotion.y);
#endif
		}
#ifdef DEBUG_XEVENTS
		if ( i == 10 ) {
			printf("Warning: didn't detect mouse warp motion\n");
		}
#endif
	}
	return(posted);
}

static int X11_DispatchEvent(_THIS)
{
	int posted;
	XEvent xevent;

	SDL_memset(&xevent, '\0', sizeof (XEvent));  
	XNextEvent(SDL_Display, &xevent);

	if ( xevent.type == KeyRelease
	     && X11_KeyRepeat(SDL_Display, &xevent) ) {
		return 0;
	}

#ifdef X_HAVE_UTF8_STRING
	if ( SDL_TranslateUNICODE
	     && SDL_IM != NULL
	     && XFilterEvent(&xevent, None) ) {
		return 0;
	}
#endif

	posted = 0;
	switch (xevent.type) {

	    
	    case EnterNotify: {
#ifdef DEBUG_XEVENTS
printf("EnterNotify! (%d,%d)\n", xevent.xcrossing.x, xevent.xcrossing.y);
if ( xevent.xcrossing.mode == NotifyGrab )
printf("Mode: NotifyGrab\n");
if ( xevent.xcrossing.mode == NotifyUngrab )
printf("Mode: NotifyUngrab\n");
#endif
		if ( this->input_grab == SDL_GRAB_OFF ) {
			posted = SDL_PrivateAppActive(1, SDL_APPMOUSEFOCUS);
		}
		posted = SDL_PrivateMouseMotion(0, 0,
				xevent.xcrossing.x,
				xevent.xcrossing.y);
	    }
	    break;

	    
	    case LeaveNotify: {
#ifdef DEBUG_XEVENTS
printf("LeaveNotify! (%d,%d)\n", xevent.xcrossing.x, xevent.xcrossing.y);
if ( xevent.xcrossing.mode == NotifyGrab )
printf("Mode: NotifyGrab\n");
if ( xevent.xcrossing.mode == NotifyUngrab )
printf("Mode: NotifyUngrab\n");
#endif
		if ( (xevent.xcrossing.mode != NotifyGrab) &&
		     (xevent.xcrossing.mode != NotifyUngrab) &&
		     (xevent.xcrossing.detail != NotifyInferior) ) {
               		if ( this->input_grab == SDL_GRAB_OFF ) {
				posted = SDL_PrivateAppActive(0, SDL_APPMOUSEFOCUS);
			} else {
				posted = SDL_PrivateMouseMotion(0, 0,
						xevent.xcrossing.x,
						xevent.xcrossing.y);
			}
		}
	    }
	    break;

	    
	    case FocusIn: {
#ifdef DEBUG_XEVENTS
printf("FocusIn!\n");
#endif
		posted = SDL_PrivateAppActive(1, SDL_APPINPUTFOCUS);

#ifdef X_HAVE_UTF8_STRING
		if ( SDL_IC != NULL ) {
			XSetICFocus(SDL_IC);
		}
#endif
		
		switch_waiting = 0x01 | SDL_FULLSCREEN;
		switch_time = SDL_GetTicks() + 1500;
	    }
	    break;

	    
	    case FocusOut: {
#ifdef DEBUG_XEVENTS
printf("FocusOut!\n");
#endif
		posted = SDL_PrivateAppActive(0, SDL_APPINPUTFOCUS);

#ifdef X_HAVE_UTF8_STRING
		if ( SDL_IC != NULL ) {
			XUnsetICFocus(SDL_IC);
		}
#endif
		
		switch_waiting = 0x01;
		switch_time = SDL_GetTicks() + 200;
	    }
	    break;

#ifdef X_HAVE_UTF8_STRING
	    case MappingNotify: {
		XRefreshKeyboardMapping(&xevent.xmapping);
	    }
	    break;
#endif 

	    
	    case KeymapNotify: {
#ifdef DEBUG_XEVENTS
printf("KeymapNotify!\n");
#endif
		X11_SetKeyboardState(SDL_Display,  xevent.xkeymap.key_vector);
	    }
	    break;

	    
	    case MotionNotify: {
		if ( SDL_VideoSurface ) {
			if ( mouse_relative ) {
				if ( using_dga & DGA_MOUSE ) {
#ifdef DEBUG_MOTION
  printf("DGA motion: %d,%d\n", xevent.xmotion.x_root, xevent.xmotion.y_root);
#endif
					posted = SDL_PrivateMouseMotion(0, 1,
							xevent.xmotion.x_root,
							xevent.xmotion.y_root);
				} else {
					posted = X11_WarpedMotion(this,&xevent);
				}
			} else {
#ifdef DEBUG_MOTION
  printf("X11 motion: %d,%d\n", xevent.xmotion.x, xevent.xmotion.y);
#endif
				posted = SDL_PrivateMouseMotion(0, 0,
						xevent.xmotion.x,
						xevent.xmotion.y);
			}
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
		KeyCode keycode = xevent.xkey.keycode;

#ifdef DEBUG_XEVENTS
printf("KeyPress (X11 keycode = 0x%X)\n", xevent.xkey.keycode);
#endif
		
		if ( !SDL_TranslateUNICODE ) {
			
			keysym.scancode = keycode;
			keysym.sym = X11_TranslateKeycode(SDL_Display, keycode);
			keysym.mod = KMOD_NONE;
			keysym.unicode = 0;
			posted = SDL_PrivateKeyboard(SDL_PRESSED, &keysym);
			break;
		}

		
#ifdef X_HAVE_UTF8_STRING
		if ( SDL_IC != NULL ) {
			Status status;
			KeySym xkeysym;
			int i;
			
			char keybuf[32];
			char *keydata = keybuf;
			int count;
			Uint16 utf16buf[32];
			Uint16 *utf16data = utf16buf;
			int utf16size;
			int utf16length;

			count = Xutf8LookupString(SDL_IC, &xevent.xkey, keydata, sizeof(keybuf), &xkeysym, &status);
			if (XBufferOverflow == status) {
			  keydata = SDL_malloc(count);
			  if ( keydata == NULL ) {
			    SDL_OutOfMemory();
			    break;
			  }
			  count = Xutf8LookupString(SDL_IC, &xevent.xkey, keydata, count, &xkeysym, &status);
			}

			switch (status) {

			case XBufferOverflow: {
			  SDL_SetError("Xutf8LookupString indicated a double buffer overflow!");
			  break;
			}

			case XLookupChars:
			case XLookupBoth: {
			  if (0 == count) {
			    break;
			  }

			  utf16size = count * sizeof(Uint16);
			  if (utf16size > sizeof(utf16buf)) {
			    utf16data = (Uint16 *) SDL_malloc(utf16size);
			    if (utf16data == NULL) {
			      SDL_OutOfMemory();
			      break;
			    }
			  }
			  utf16length = Utf8ToUtf16((Uint8 *)keydata, count, utf16data, utf16size);
			  if (utf16length < 0) {
			    SDL_SetError("Oops! Xutf8LookupString returned an invalid UTF-8 sequence!");
			    break;
			  }

			  for (i = 0; i < utf16length - 1; i++) {
			    keysym.scancode = 0;
			    keysym.sym = SDLK_UNKNOWN;
			    keysym.mod = KMOD_NONE;
			    keysym.unicode = utf16data[i];
			    posted = SDL_PrivateKeyboard(SDL_PRESSED, &keysym);
			  }
			  if (utf16length > 0) {			       
			    keysym.scancode = keycode;
			    keysym.sym = (keycode ? X11_TranslateKeycode(SDL_Display, keycode) : 0);
			    keysym.mod = KMOD_NONE;
			    keysym.unicode = utf16data[utf16length - 1];
			    posted = SDL_PrivateKeyboard(SDL_PRESSED, &keysym);
			  }
			  break;
			}

			case XLookupKeySym: {
			  if (keycode) {
			    keysym.scancode = keycode;
			    keysym.sym = X11_TranslateKeycode(SDL_Display, keycode);
			    keysym.mod = KMOD_NONE;
			    keysym.unicode = 0;
			    posted = SDL_PrivateKeyboard(SDL_PRESSED, &keysym);
			  }
			  break;
			}

			case XLookupNone: {
			  
			  break;
			}

			default:
			  
			  SDL_SetError("Oops! Xutf8LookupStringreturned an unknown status");
			}

			
			if (keydata != NULL && keybuf != keydata) {
			  SDL_free(keydata);
			}
			if (utf16data != NULL && utf16buf != utf16data) {
			  SDL_free(utf16data);
			}
		}
		else
#endif
		{
			static XComposeStatus state;
			char keybuf[32];

			keysym.scancode = keycode;
			keysym.sym = X11_TranslateKeycode(SDL_Display, keycode);
			keysym.mod = KMOD_NONE;
			keysym.unicode = 0;
			if ( XLookupString(&xevent.xkey,
			                    keybuf, sizeof(keybuf),
			                    NULL, &state) ) {
				keysym.unicode = (Uint8)keybuf[0];
			}

			posted = SDL_PrivateKeyboard(SDL_PRESSED, &keysym);
		}
	    }
	    break;

	    
	    case KeyRelease: {
		SDL_keysym keysym;
		KeyCode keycode = xevent.xkey.keycode;

		if (keycode == 0) {
		  
		  break;
		}

#ifdef DEBUG_XEVENTS
printf("KeyRelease (X11 keycode = 0x%X)\n", xevent.xkey.keycode);
#endif

		
		keysym.scancode = keycode;
		keysym.sym = X11_TranslateKeycode(SDL_Display, keycode);
		keysym.mod = KMOD_NONE;
		keysym.unicode = 0;

		posted = SDL_PrivateKeyboard(SDL_RELEASED, &keysym);
	    }
	    break;

	    
	    case UnmapNotify: {
#ifdef DEBUG_XEVENTS
printf("UnmapNotify!\n");
#endif
		
		if ( SDL_GetAppState() & SDL_APPACTIVE ) {
			
			X11_SwapVidModeGamma(this);

			
			posted = SDL_PrivateAppActive(0,
					SDL_APPACTIVE|SDL_APPINPUTFOCUS);
		}
	    }
	    break;

	    
	    case MapNotify: {
#ifdef DEBUG_XEVENTS
printf("MapNotify!\n");
#endif
		
		if ( !(SDL_GetAppState() & SDL_APPACTIVE) ) {
			
			posted = SDL_PrivateAppActive(1, SDL_APPACTIVE);

			
			X11_SwapVidModeGamma(this);
		}

		if ( SDL_VideoSurface &&
		     (SDL_VideoSurface->flags & SDL_FULLSCREEN) ) {
			X11_EnterFullScreen(this);
		} else {
			X11_GrabInputNoLock(this, this->input_grab);
		}
		X11_CheckMouseModeNoLock(this);

		if ( SDL_VideoSurface ) {
			X11_RefreshDisplay(this);
		}
	    }
	    break;

	    
	    case ConfigureNotify: {
#ifdef DEBUG_XEVENTS
printf("ConfigureNotify! (resize: %dx%d)\n", xevent.xconfigure.width, xevent.xconfigure.height);
#endif
		if ((X11_PendingConfigureNotifyWidth != -1) &&
		    (X11_PendingConfigureNotifyHeight != -1)) {
		    if ((xevent.xconfigure.width != X11_PendingConfigureNotifyWidth) &&
			(xevent.xconfigure.height != X11_PendingConfigureNotifyHeight)) {
			    
			    break;
		    }
		    X11_PendingConfigureNotifyWidth = -1;
		    X11_PendingConfigureNotifyHeight = -1;
		}
		if ( SDL_VideoSurface ) {
		    if ((xevent.xconfigure.width != SDL_VideoSurface->w) ||
		        (xevent.xconfigure.height != SDL_VideoSurface->h)) {
			
			if ( ! ((xevent.xconfigure.width == 32) &&
			        (xevent.xconfigure.height == 32)) ) {
				SDL_PrivateResize(xevent.xconfigure.width,
				                  xevent.xconfigure.height);
			}
		    } else {
			
			if ( SDL_VideoSurface->flags & SDL_OPENGL ) {
				SDL_PrivateExpose();
			}
		    }
		}
	    }
	    break;

	    
	    case ClientMessage: {
		if ( (xevent.xclient.format == 32) &&
		     (xevent.xclient.data.l[0] == WM_DELETE_WINDOW) )
		{
			posted = SDL_PrivateQuit();
		} else
		if ( SDL_ProcessEvents[SDL_SYSWMEVENT] == SDL_ENABLE ) {
			SDL_SysWMmsg wmmsg;

			SDL_VERSION(&wmmsg.version);
			wmmsg.subsystem = SDL_SYSWM_X11;
			wmmsg.event.xevent = xevent;
			posted = SDL_PrivateSysWMEvent(&wmmsg);
		}
	    }
	    break;

	    
	    case Expose: {
#ifdef DEBUG_XEVENTS
printf("Expose (count = %d)\n", xevent.xexpose.count);
#endif
		if ( SDL_VideoSurface && (xevent.xexpose.count == 0) ) {
			X11_RefreshDisplay(this);
		}
	    }
	    break;

	    default: {
#ifdef DEBUG_XEVENTS
printf("Unhandled event %d\n", xevent.type);
#endif
		
		if ( SDL_ProcessEvents[SDL_SYSWMEVENT] == SDL_ENABLE ) {
			SDL_SysWMmsg wmmsg;

			SDL_VERSION(&wmmsg.version);
			wmmsg.subsystem = SDL_SYSWM_X11;
			wmmsg.event.xevent = xevent;
			posted = SDL_PrivateSysWMEvent(&wmmsg);
		}
	    }
	    break;
	}
	return(posted);
}

int X11_Pending(Display *display)
{
	
	XFlush(display);
	if ( XEventsQueued(display, QueuedAlready) ) {
		return(1);
	}

	
	{
		static struct timeval zero_time;	
		int x11_fd;
		fd_set fdset;

		x11_fd = ConnectionNumber(display);
		FD_ZERO(&fdset);
		FD_SET(x11_fd, &fdset);
		if ( select(x11_fd+1, &fdset, NULL, NULL, &zero_time) == 1 ) {
			return(XPending(display));
		}
	}

	
	return(0);
}

void X11_PumpEvents(_THIS)
{
	int pending;

	
	if (!allow_screensaver) {
		static Uint32 screensaverTicks;
		Uint32 nowTicks = SDL_GetTicks();
		if ((nowTicks - screensaverTicks) > 5000) {
			XResetScreenSaver(SDL_Display);
			screensaverTicks = nowTicks;
		}
	}

	
	pending = 0;
	while ( X11_Pending(SDL_Display) ) {
		X11_DispatchEvent(this);
		++pending;
	}
	if ( switch_waiting ) {
		Uint32 now;

		now  = SDL_GetTicks();
		if ( pending || !SDL_VideoSurface ) {
			
			if ( switch_waiting & SDL_FULLSCREEN ) {
				switch_time = now + 1500;
			} else {
				switch_time = now + 200;
			}
		} else if ( (int)(switch_time-now) <= 0 ) {
			Uint32 go_fullscreen;

			go_fullscreen = switch_waiting & SDL_FULLSCREEN;
			switch_waiting = 0;
			if ( SDL_VideoSurface->flags & SDL_FULLSCREEN ) {
				if ( go_fullscreen ) {
					X11_EnterFullScreen(this);
				} else {
					X11_LeaveFullScreen(this);
				}
			}
			
			if ( go_fullscreen ) {
				X11_GrabInputNoLock(this, this->input_grab);
			} else {
				X11_GrabInputNoLock(this, SDL_GRAB_OFF);
			}
			X11_CheckMouseModeNoLock(this);
		}
	}
}

void X11_InitKeymap(void)
{
	int i;

	
	for ( i=0; i<SDL_arraysize(ODD_keymap); ++i )
		ODD_keymap[i] = SDLK_UNKNOWN;

 	
 	ODD_keymap[XK_dead_grave&0xFF] = SDLK_COMPOSE;
 	ODD_keymap[XK_dead_acute&0xFF] = SDLK_COMPOSE;
 	ODD_keymap[XK_dead_tilde&0xFF] = SDLK_COMPOSE;
 	ODD_keymap[XK_dead_macron&0xFF] = SDLK_COMPOSE;
 	ODD_keymap[XK_dead_breve&0xFF] = SDLK_COMPOSE;
 	ODD_keymap[XK_dead_abovedot&0xFF] = SDLK_COMPOSE;
 	ODD_keymap[XK_dead_diaeresis&0xFF] = SDLK_COMPOSE;
 	ODD_keymap[XK_dead_abovering&0xFF] = SDLK_COMPOSE;
 	ODD_keymap[XK_dead_doubleacute&0xFF] = SDLK_COMPOSE;
 	ODD_keymap[XK_dead_caron&0xFF] = SDLK_COMPOSE;
 	ODD_keymap[XK_dead_cedilla&0xFF] = SDLK_COMPOSE;
 	ODD_keymap[XK_dead_ogonek&0xFF] = SDLK_COMPOSE;
 	ODD_keymap[XK_dead_iota&0xFF] = SDLK_COMPOSE;
 	ODD_keymap[XK_dead_voiced_sound&0xFF] = SDLK_COMPOSE;
 	ODD_keymap[XK_dead_semivoiced_sound&0xFF] = SDLK_COMPOSE;
 	ODD_keymap[XK_dead_belowdot&0xFF] = SDLK_COMPOSE;
#ifdef XK_dead_hook
 	ODD_keymap[XK_dead_hook&0xFF] = SDLK_COMPOSE;
#endif
#ifdef XK_dead_horn
 	ODD_keymap[XK_dead_horn&0xFF] = SDLK_COMPOSE;
#endif

#ifdef XK_dead_circumflex
	
	ODD_keymap[XK_dead_circumflex&0xFF] = SDLK_CARET;
#endif
#ifdef XK_ISO_Level3_Shift
	ODD_keymap[XK_ISO_Level3_Shift&0xFF] = SDLK_MODE; 
#endif

	
	for ( i=0; i<SDL_arraysize(MISC_keymap); ++i )
		MISC_keymap[i] = SDLK_UNKNOWN;

	
	MISC_keymap[XK_BackSpace&0xFF] = SDLK_BACKSPACE;
	MISC_keymap[XK_Tab&0xFF] = SDLK_TAB;
	MISC_keymap[XK_Clear&0xFF] = SDLK_CLEAR;
	MISC_keymap[XK_Return&0xFF] = SDLK_RETURN;
	MISC_keymap[XK_Pause&0xFF] = SDLK_PAUSE;
	MISC_keymap[XK_Escape&0xFF] = SDLK_ESCAPE;
	MISC_keymap[XK_Delete&0xFF] = SDLK_DELETE;

	MISC_keymap[XK_KP_0&0xFF] = SDLK_KP0;		
	MISC_keymap[XK_KP_1&0xFF] = SDLK_KP1;
	MISC_keymap[XK_KP_2&0xFF] = SDLK_KP2;
	MISC_keymap[XK_KP_3&0xFF] = SDLK_KP3;
	MISC_keymap[XK_KP_4&0xFF] = SDLK_KP4;
	MISC_keymap[XK_KP_5&0xFF] = SDLK_KP5;
	MISC_keymap[XK_KP_6&0xFF] = SDLK_KP6;
	MISC_keymap[XK_KP_7&0xFF] = SDLK_KP7;
	MISC_keymap[XK_KP_8&0xFF] = SDLK_KP8;
	MISC_keymap[XK_KP_9&0xFF] = SDLK_KP9;
	MISC_keymap[XK_KP_Insert&0xFF] = SDLK_KP0;
	MISC_keymap[XK_KP_End&0xFF] = SDLK_KP1;	
	MISC_keymap[XK_KP_Down&0xFF] = SDLK_KP2;
	MISC_keymap[XK_KP_Page_Down&0xFF] = SDLK_KP3;
	MISC_keymap[XK_KP_Left&0xFF] = SDLK_KP4;
	MISC_keymap[XK_KP_Begin&0xFF] = SDLK_KP5;
	MISC_keymap[XK_KP_Right&0xFF] = SDLK_KP6;
	MISC_keymap[XK_KP_Home&0xFF] = SDLK_KP7;
	MISC_keymap[XK_KP_Up&0xFF] = SDLK_KP8;
	MISC_keymap[XK_KP_Page_Up&0xFF] = SDLK_KP9;
	MISC_keymap[XK_KP_Delete&0xFF] = SDLK_KP_PERIOD;
	MISC_keymap[XK_KP_Decimal&0xFF] = SDLK_KP_PERIOD;
	MISC_keymap[XK_KP_Divide&0xFF] = SDLK_KP_DIVIDE;
	MISC_keymap[XK_KP_Multiply&0xFF] = SDLK_KP_MULTIPLY;
	MISC_keymap[XK_KP_Subtract&0xFF] = SDLK_KP_MINUS;
	MISC_keymap[XK_KP_Add&0xFF] = SDLK_KP_PLUS;
	MISC_keymap[XK_KP_Enter&0xFF] = SDLK_KP_ENTER;
	MISC_keymap[XK_KP_Equal&0xFF] = SDLK_KP_EQUALS;

	MISC_keymap[XK_Up&0xFF] = SDLK_UP;
	MISC_keymap[XK_Down&0xFF] = SDLK_DOWN;
	MISC_keymap[XK_Right&0xFF] = SDLK_RIGHT;
	MISC_keymap[XK_Left&0xFF] = SDLK_LEFT;
	MISC_keymap[XK_Insert&0xFF] = SDLK_INSERT;
	MISC_keymap[XK_Home&0xFF] = SDLK_HOME;
	MISC_keymap[XK_End&0xFF] = SDLK_END;
	MISC_keymap[XK_Page_Up&0xFF] = SDLK_PAGEUP;
	MISC_keymap[XK_Page_Down&0xFF] = SDLK_PAGEDOWN;

	MISC_keymap[XK_F1&0xFF] = SDLK_F1;
	MISC_keymap[XK_F2&0xFF] = SDLK_F2;
	MISC_keymap[XK_F3&0xFF] = SDLK_F3;
	MISC_keymap[XK_F4&0xFF] = SDLK_F4;
	MISC_keymap[XK_F5&0xFF] = SDLK_F5;
	MISC_keymap[XK_F6&0xFF] = SDLK_F6;
	MISC_keymap[XK_F7&0xFF] = SDLK_F7;
	MISC_keymap[XK_F8&0xFF] = SDLK_F8;
	MISC_keymap[XK_F9&0xFF] = SDLK_F9;
	MISC_keymap[XK_F10&0xFF] = SDLK_F10;
	MISC_keymap[XK_F11&0xFF] = SDLK_F11;
	MISC_keymap[XK_F12&0xFF] = SDLK_F12;
	MISC_keymap[XK_F13&0xFF] = SDLK_F13;
	MISC_keymap[XK_F14&0xFF] = SDLK_F14;
	MISC_keymap[XK_F15&0xFF] = SDLK_F15;

	MISC_keymap[XK_Num_Lock&0xFF] = SDLK_NUMLOCK;
	MISC_keymap[XK_Caps_Lock&0xFF] = SDLK_CAPSLOCK;
	MISC_keymap[XK_Scroll_Lock&0xFF] = SDLK_SCROLLOCK;
	MISC_keymap[XK_Shift_R&0xFF] = SDLK_RSHIFT;
	MISC_keymap[XK_Shift_L&0xFF] = SDLK_LSHIFT;
	MISC_keymap[XK_Control_R&0xFF] = SDLK_RCTRL;
	MISC_keymap[XK_Control_L&0xFF] = SDLK_LCTRL;
	MISC_keymap[XK_Alt_R&0xFF] = SDLK_RALT;
	MISC_keymap[XK_Alt_L&0xFF] = SDLK_LALT;
	MISC_keymap[XK_Meta_R&0xFF] = SDLK_RMETA;
	MISC_keymap[XK_Meta_L&0xFF] = SDLK_LMETA;
	MISC_keymap[XK_Super_L&0xFF] = SDLK_LSUPER; 
	MISC_keymap[XK_Super_R&0xFF] = SDLK_RSUPER; 
	MISC_keymap[XK_Mode_switch&0xFF] = SDLK_MODE; 
	MISC_keymap[XK_Multi_key&0xFF] = SDLK_COMPOSE; 

	MISC_keymap[XK_Help&0xFF] = SDLK_HELP;
	MISC_keymap[XK_Print&0xFF] = SDLK_PRINT;
	MISC_keymap[XK_Sys_Req&0xFF] = SDLK_SYSREQ;
	MISC_keymap[XK_Break&0xFF] = SDLK_BREAK;
	MISC_keymap[XK_Menu&0xFF] = SDLK_MENU;
	MISC_keymap[XK_Hyper_R&0xFF] = SDLK_MENU;   
}

SDLKey X11_TranslateKeycode(Display *display, KeyCode kc)
{
	KeySym xsym;
	SDLKey key;

	xsym = XKeycodeToKeysym(display, kc, 0);
#ifdef DEBUG_KEYS
	fprintf(stderr, "Translating key code %d -> 0x%.4x\n", kc, xsym);
#endif
	key = SDLK_UNKNOWN;
	if ( xsym ) {
		switch (xsym>>8) {
		    case 0x1005FF:
#ifdef SunXK_F36
			if ( xsym == SunXK_F36 )
				key = SDLK_F11;
#endif
#ifdef SunXK_F37
			if ( xsym == SunXK_F37 )
				key = SDLK_F12;
#endif
			break;
		    case 0x00:	
			key = (SDLKey)(xsym & 0xFF);
			break;
		    case 0x01:	
		    case 0x02:	
		    case 0x03:	
		    case 0x04:	
		    case 0x05:	
		    case 0x06:	
		    case 0x07:	
		    case 0x08:	
		    case 0x0A:	
		    case 0x0C:	
		    case 0x0D:	
			
			key = (SDLKey)(xsym & 0xFF);
			break;
		    case 0xFE:
			key = ODD_keymap[xsym&0xFF];
			break;
		    case 0xFF:
			key = MISC_keymap[xsym&0xFF];
			break;
		    default:
			break;
		}
	} else {
		
		switch (kc) {
		    case 115:
			key = SDLK_LSUPER;
			break;
		    case 116:
			key = SDLK_RSUPER;
			break;
		    case 117:
			key = SDLK_MENU;
			break;
		    default:
			break;
		}
	}
	return key;
}

static unsigned meta_l_mask, meta_r_mask, alt_l_mask, alt_r_mask;
static unsigned num_mask, mode_switch_mask;

static void get_modifier_masks(Display *display)
{
	static unsigned got_masks;
	int i, j;
	XModifierKeymap *xmods;
	unsigned n;

	if(got_masks)
		return;

	xmods = XGetModifierMapping(display);
	n = xmods->max_keypermod;
	for(i = 3; i < 8; i++) {
		for(j = 0; j < n; j++) {
			KeyCode kc = xmods->modifiermap[i * n + j];
			KeySym ks = XKeycodeToKeysym(display, kc, 0);
			unsigned mask = 1 << i;
			switch(ks) {
			case XK_Num_Lock:
				num_mask = mask; break;
			case XK_Alt_L:
				alt_l_mask = mask; break;
			case XK_Alt_R:
				alt_r_mask = mask; break;
			case XK_Meta_L:
				meta_l_mask = mask; break;
			case XK_Meta_R:
				meta_r_mask = mask; break;
			case XK_Mode_switch:
				mode_switch_mask = mask; break;
			}
		}
	}
	XFreeModifiermap(xmods);
	got_masks = 1;
}


Uint16 X11_KeyToUnicode(SDLKey keysym, SDLMod modifiers)
{
	struct SDL_VideoDevice *this = current_video;
	char keybuf[32];
	int i;
	KeySym xsym = 0;
	XKeyEvent xkey;
	Uint16 unicode;

	if ( !this || !SDL_Display ) {
		return 0;
	}

	SDL_memset(&xkey, 0, sizeof(xkey));
	xkey.display = SDL_Display;

	xsym = keysym;		
	for (i = 0; i < 256; ++i) {
		if ( MISC_keymap[i] == keysym ) {
			xsym = 0xFF00 | i;
			break;
		} else if ( ODD_keymap[i] == keysym ) {
			xsym = 0xFE00 | i;
			break;
		}
	}

	xkey.keycode = XKeysymToKeycode(xkey.display, xsym);

	get_modifier_masks(SDL_Display);
	if(modifiers & KMOD_SHIFT)
		xkey.state |= ShiftMask;
	if(modifiers & KMOD_CAPS)
		xkey.state |= LockMask;
	if(modifiers & KMOD_CTRL)
		xkey.state |= ControlMask;
	if(modifiers & KMOD_MODE)
		xkey.state |= mode_switch_mask;
	if(modifiers & KMOD_LALT)
		xkey.state |= alt_l_mask;
	if(modifiers & KMOD_RALT)
		xkey.state |= alt_r_mask;
	if(modifiers & KMOD_LMETA)
		xkey.state |= meta_l_mask;
	if(modifiers & KMOD_RMETA)
		xkey.state |= meta_r_mask;
	if(modifiers & KMOD_NUM)
		xkey.state |= num_mask;

	unicode = 0;
	if ( XLookupString(&xkey, keybuf, sizeof(keybuf), NULL, NULL) )
		unicode = (unsigned char)keybuf[0];
	return(unicode);
}


void X11_SetKeyboardState(Display *display, const char *key_vec)
{
	char keys_return[32];
	int i;
	Uint8 *kstate = SDL_GetKeyState(NULL);
	SDLMod modstate;
	Window junk_window;
	int x, y;
	unsigned int mask;

	
	if ( ! key_vec ) {
		XQueryKeymap(display, keys_return);
		key_vec = keys_return;
	}

	
	modstate = 0;
	get_modifier_masks(display);
	if ( XQueryPointer(display, DefaultRootWindow(display),
		&junk_window, &junk_window, &x, &y, &x, &y, &mask) ) {
		if ( mask & LockMask ) {
			modstate |= KMOD_CAPS;
		}
		if ( mask & mode_switch_mask ) {
			modstate |= KMOD_MODE;
		}
		if ( mask & num_mask ) {
			modstate |= KMOD_NUM;
		}
	}

	
	SDL_memset(kstate, 0, SDLK_LAST);
	for ( i = 0; i < 32; i++ ) {
		int j;
		if ( !key_vec[i] )
			continue;
		for ( j = 0; j < 8; j++ ) {
			if ( key_vec[i] & (1 << j) ) {
				SDLKey key;
				KeyCode kc = (i << 3 | j);
				key = X11_TranslateKeycode(display, kc);
				if ( key == SDLK_UNKNOWN ) {
					continue;
				}
				kstate[key] = SDL_PRESSED;
				switch (key) {
				    case SDLK_LSHIFT:
					modstate |= KMOD_LSHIFT;
					break;
				    case SDLK_RSHIFT:
					modstate |= KMOD_RSHIFT;
					break;
				    case SDLK_LCTRL:
					modstate |= KMOD_LCTRL;
					break;
				    case SDLK_RCTRL:
					modstate |= KMOD_RCTRL;
					break;
				    case SDLK_LALT:
					modstate |= KMOD_LALT;
					break;
				    case SDLK_RALT:
					modstate |= KMOD_RALT;
					break;
				    case SDLK_LMETA:
					modstate |= KMOD_LMETA;
					break;
				    case SDLK_RMETA:
					modstate |= KMOD_RMETA;
					break;
				    default:
					break;
				}
			}
		}
	}

	
	if ( modstate & KMOD_CAPS ) {
		kstate[SDLK_CAPSLOCK] = SDL_PRESSED;
	} else {
		kstate[SDLK_CAPSLOCK] = SDL_RELEASED;
	}
	if ( modstate & KMOD_NUM ) {
		kstate[SDLK_NUMLOCK] = SDL_PRESSED;
	} else {
		kstate[SDLK_NUMLOCK] = SDL_RELEASED;
	}

	
	SDL_SetModState(modstate);
}

void X11_InitOSKeymap(_THIS)
{
	X11_InitKeymap();
}


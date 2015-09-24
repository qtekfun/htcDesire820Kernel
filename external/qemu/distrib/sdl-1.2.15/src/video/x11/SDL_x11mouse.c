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

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "SDL_mouse.h"
#include "../../events/SDL_events_c.h"
#include "../SDL_cursor_c.h"
#include "SDL_x11dga_c.h"
#include "SDL_x11mouse_c.h"


struct WMcursor {
	Cursor x_cursor;
};


void X11_FreeWMCursor(_THIS, WMcursor *cursor)
{
	if ( SDL_Display != NULL ) {
		SDL_Lock_EventThread();
		XFreeCursor(SDL_Display, cursor->x_cursor);
		XSync(SDL_Display, False);
		SDL_Unlock_EventThread();
	}
	SDL_free(cursor);
}

WMcursor *X11_CreateWMCursor(_THIS,
		Uint8 *data, Uint8 *mask, int w, int h, int hot_x, int hot_y)
{
	WMcursor *cursor;
	XGCValues GCvalues;
	GC        GCcursor;
	XImage *data_image, *mask_image;
	Pixmap  data_pixmap, mask_pixmap;
	int       clen, i;
	char     *x_data, *x_mask;
	static XColor black = {  0,  0,  0,  0 };
	static XColor white = { 0xffff, 0xffff, 0xffff, 0xffff };

	
	cursor = (WMcursor *)SDL_malloc(sizeof(WMcursor));
	if ( cursor == NULL ) {
		SDL_OutOfMemory();
		return(NULL);
	}

	
	clen = (w/8)*h;
	x_data = (char *)SDL_malloc(clen);
	if ( x_data == NULL ) {
		SDL_free(cursor);
		SDL_OutOfMemory();
		return(NULL);
	}
	x_mask = (char *)SDL_malloc(clen);
	if ( x_mask == NULL ) {
		SDL_free(cursor);
		SDL_free(x_data);
		SDL_OutOfMemory();
		return(NULL);
	}
	for ( i=0; i<clen; ++i ) {
		x_mask[i] = data[i] | mask[i];
		x_data[i] = data[i];
	}

	
	SDL_Lock_EventThread();

	
	data_image = XCreateImage(SDL_Display, 
			DefaultVisual(SDL_Display, SDL_Screen),
					1, XYBitmap, 0, x_data, w, h, 8, w/8);
	data_image->byte_order = MSBFirst;
	data_image->bitmap_bit_order = MSBFirst;
	data_pixmap = XCreatePixmap(SDL_Display, SDL_Root, w, h, 1);

	
	mask_image = XCreateImage(SDL_Display, 
			DefaultVisual(SDL_Display, SDL_Screen),
					1, XYBitmap, 0, x_mask, w, h, 8, w/8);
	mask_image->byte_order = MSBFirst;
	mask_image->bitmap_bit_order = MSBFirst;
	mask_pixmap = XCreatePixmap(SDL_Display, SDL_Root, w, h, 1);

	
	GCvalues.function = GXcopy;
	GCvalues.foreground = ~0;
	GCvalues.background =  0;
	GCvalues.plane_mask = AllPlanes;
	GCcursor = XCreateGC(SDL_Display, data_pixmap,
			(GCFunction|GCForeground|GCBackground|GCPlaneMask),
								&GCvalues);

	
	XPutImage(SDL_Display, data_pixmap, GCcursor, data_image,
							0, 0, 0, 0, w, h);
	XPutImage(SDL_Display, mask_pixmap, GCcursor, mask_image,
							0, 0, 0, 0, w, h);
	XFreeGC(SDL_Display, GCcursor);
	
	XDestroyImage(data_image);
	XDestroyImage(mask_image);

	
	cursor->x_cursor = XCreatePixmapCursor(SDL_Display, data_pixmap,
				mask_pixmap, &black, &white, hot_x, hot_y);
	XFreePixmap(SDL_Display, data_pixmap);
	XFreePixmap(SDL_Display, mask_pixmap);

	
	XSync(SDL_Display, False);
	SDL_Unlock_EventThread();

	return(cursor);
}

int X11_ShowWMCursor(_THIS, WMcursor *cursor)
{
	
	if ( SDL_Display == NULL ) {
		return(0);
	}

	
	if ( SDL_Window ) {
		SDL_Lock_EventThread();
		if ( cursor == NULL ) {
			if ( SDL_BlankCursor != NULL ) {
				XDefineCursor(SDL_Display, SDL_Window,
					SDL_BlankCursor->x_cursor);
			}
		} else {
			XDefineCursor(SDL_Display, SDL_Window, cursor->x_cursor);
		}
		XSync(SDL_Display, False);
		SDL_Unlock_EventThread();
	}
	return(1);
}

void X11_WarpWMCursor(_THIS, Uint16 x, Uint16 y)
{
	if ( using_dga & DGA_MOUSE ) {
		SDL_PrivateMouseMotion(0, 0, x, y);
	} else if ( mouse_relative) {
		SDL_PrivateMouseMotion(0, 0, x, y);
	} else {
		SDL_Lock_EventThread();
		XWarpPointer(SDL_Display, None, SDL_Window, 0, 0, 0, 0, x, y);
		XSync(SDL_Display, False);
		SDL_Unlock_EventThread();
	}
}

static void SetMouseAccel(_THIS, const char *accel_param)
{
	int i;
	size_t len;
	int accel_value[3];
	char *mouse_param, *mouse_param_buf, *pin;

	len = SDL_strlen(accel_param)+1;
	mouse_param_buf = SDL_stack_alloc(char, len);
	if ( ! mouse_param_buf ) {
		return;
	}
	SDL_strlcpy(mouse_param_buf, accel_param, len);
	mouse_param = mouse_param_buf;

	for ( i=0; (i < 3) && mouse_param; ++i ) {
		pin = SDL_strchr(mouse_param, '/');
		if ( pin ) {
			*pin = '\0';
		}
		accel_value[i] = atoi(mouse_param);
		if ( pin ) {
			mouse_param = pin+1;
		} else {
			mouse_param = NULL;
		}
	}
	if ( i == 3 ) {
		XChangePointerControl(SDL_Display, True, True,
			accel_value[0], accel_value[1], accel_value[2]);
	}
	SDL_stack_free(mouse_param_buf);
}

void X11_CheckMouseModeNoLock(_THIS)
{
	const Uint8 full_focus = (SDL_APPACTIVE|SDL_APPINPUTFOCUS|SDL_APPMOUSEFOCUS);
	char *env_override;
	int enable_relative = 1;

	
	if ( SDL_Display == NULL )
	        return;

	env_override = SDL_getenv("SDL_MOUSE_RELATIVE");
	if ( env_override ) {
		enable_relative = atoi(env_override);
	}

	
	if ( enable_relative &&
	     !(SDL_cursorstate & CURSOR_VISIBLE) &&
	     (this->input_grab != SDL_GRAB_OFF) &&
             (SDL_GetAppState() & full_focus) == full_focus ) {
		if ( ! mouse_relative ) {
			X11_EnableDGAMouse(this);
			if ( ! (using_dga & DGA_MOUSE) ) {
				char *xmouse_accel;

				SDL_GetMouseState(&mouse_last.x, &mouse_last.y);
				
				XGetPointerControl(SDL_Display,
						&mouse_accel.numerator, 
						&mouse_accel.denominator,
						&mouse_accel.threshold);
				xmouse_accel=SDL_getenv("SDL_VIDEO_X11_MOUSEACCEL");
				if ( xmouse_accel ) {
					SetMouseAccel(this, xmouse_accel);
				}
			}
			mouse_relative = 1;
		}
	} else {
		if ( mouse_relative ) {
			if ( using_dga & DGA_MOUSE ) {
				X11_DisableDGAMouse(this);
			} else {
				XChangePointerControl(SDL_Display, True, True,
						mouse_accel.numerator, 
						mouse_accel.denominator,
						mouse_accel.threshold);
			}
			mouse_relative = 0;
		}
	}
}
void X11_CheckMouseMode(_THIS)
{
	SDL_Lock_EventThread();
	X11_CheckMouseModeNoLock(this);
	SDL_Unlock_EventThread();
}

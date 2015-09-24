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

#ifndef _SDL_gemvideo_h
#define _SDL_gemvideo_h

#include "SDL_mutex.h"
#include "../SDL_sysvideo.h"

struct WMcursor {
	MFORM *mform_p;
};

#define _THIS	SDL_VideoDevice *this

void GEM_wind_redraw(_THIS, int winhandle, short *inside);


#define B2S_C2P_1TO2		(1<<0)	
#define B2S_C2P_1TOS		(1<<1)	
#define B2S_VROCPYFM_1TOS	(1<<2)	
#define B2S_VROCPYFM_2TOS	(1<<3)	

#define SDL_NUMMODES	1		

struct SDL_PrivateVideoData {
	Uint16	buf2scr_ops;		
	void *buffer1;				
	void *buffer2;

	
	short vdi_handle;			
	short full_w, full_h;		
	short bpp;					
	short pixelsize;			
	short old_numcolors;		
	Uint16 pitch;				
	Uint16 format;				
	void *screen;				
	Uint32 red, green, blue, alpha;	
	Uint32 screensize;
	short	blit_coords[8];		
	MFDB	src_mfdb, dst_mfdb;	
	Uint16 old_palette[256][3];	
	Uint16 cur_palette[256][3];	
								
	void (*setpalette)(_THIS, Uint16 newpal[256][3]);

	
	short desk_x, desk_y;		
	short desk_w, desk_h;
	short win_handle;			
	int window_type;			
	const char *title_name;		
	const char *icon_name;		
	short version;				
	short wfeatures;			
	SDL_bool refresh_name;		
	SDL_bool window_fulled;		
	SDL_bool mouse_relative;	
	SDL_bool locked;			
	SDL_bool lock_redraw;		
	short message[8];			
	void *menubar;				
	SDL_bool use_dev_mouse;		
	WMcursor *cursor;			

	SDL_bool fullscreen;		
	SDL_Rect *SDL_modelist[SDL_NUMMODES+1];	
	SDL_Surface *icon;			
};

#define VDI_handle			(this->hidden->vdi_handle)
#define VDI_w				(this->hidden->full_w)
#define VDI_h				(this->hidden->full_h)
#define VDI_bpp				(this->hidden->bpp)
#define VDI_pixelsize		(this->hidden->pixelsize)
#define VDI_oldnumcolors	(this->hidden->old_numcolors)
#define VDI_oldpalette		(this->hidden->old_palette)
#define VDI_curpalette		(this->hidden->cur_palette)
#define VDI_setpalette		(this->hidden->setpalette)
#define VDI_pitch			(this->hidden->pitch)
#define VDI_format			(this->hidden->format)
#define VDI_screen			(this->hidden->screen)
#define VDI_redmask			(this->hidden->red)
#define VDI_greenmask		(this->hidden->green)
#define VDI_bluemask		(this->hidden->blue)
#define VDI_alphamask		(this->hidden->alpha)
#define VDI_screensize		(this->hidden->screensize)
#define VDI_src_mfdb		(this->hidden->src_mfdb)
#define VDI_dst_mfdb		(this->hidden->dst_mfdb)
#define VDI_blit_coords		(this->hidden->blit_coords)

#define GEM_desk_x			(this->hidden->desk_x)
#define GEM_desk_y			(this->hidden->desk_y)
#define GEM_desk_w			(this->hidden->desk_w)
#define GEM_desk_h			(this->hidden->desk_h)
#define GEM_handle			(this->hidden->win_handle)
#define GEM_win_type		(this->hidden->window_type)
#define GEM_title_name		(this->hidden->title_name)
#define GEM_icon_name		(this->hidden->icon_name)
#define GEM_refresh_name	(this->hidden->refresh_name)
#define GEM_version			(this->hidden->version)
#define GEM_wfeatures		(this->hidden->wfeatures)
#define GEM_win_fulled		(this->hidden->window_fulled)
#define GEM_mouse_relative	(this->hidden->mouse_relative)
#define GEM_locked			(this->hidden->locked)
#define GEM_lock_redraw		(this->hidden->lock_redraw)
#define GEM_message			(this->hidden->message)
#define SDL_modelist		(this->hidden->SDL_modelist)
#define GEM_icon			(this->hidden->icon)
#define GEM_fullscreen		(this->hidden->fullscreen)
#define GEM_menubar			(this->hidden->menubar)
#define GEM_usedevmouse		(this->hidden->use_dev_mouse)
#define GEM_cursor			(this->hidden->cursor)

#define GEM_buffer1			(this->hidden->buffer1)
#define GEM_buffer2			(this->hidden->buffer2)
#define GEM_bufops			(this->hidden->buf2scr_ops)

#define VDI_FBMASK(amask, rmask, gmask, bmask) \
	VDI_alphamask = (amask); \
	VDI_redmask = (rmask); \
	VDI_greenmask = (gmask); \
	VDI_bluemask = (bmask);


#endif 

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

#ifndef _SDL_sysvideo_h
#define _SDL_sysvideo_h

#include "SDL_mouse.h"
#define SDL_PROTOTYPES_ONLY
#include "SDL_syswm.h"
#undef SDL_PROTOTYPES_ONLY


#if SDL_VIDEO_OPENGL
#include "SDL_opengl.h"
#endif 

typedef struct SDL_VideoDevice SDL_VideoDevice;

#define _THIS	SDL_VideoDevice *_this
#ifndef _STATUS
#define _STATUS	SDL_status *status
#endif
struct SDL_VideoDevice {
	
	
	const char *name;

	
	

	int (*VideoInit)(_THIS, SDL_PixelFormat *vformat);

	SDL_Rect **(*ListModes)(_THIS, SDL_PixelFormat *format, Uint32 flags);

	SDL_Surface *(*SetVideoMode)(_THIS, SDL_Surface *current,
				int width, int height, int bpp, Uint32 flags);

	
	int (*ToggleFullScreen)(_THIS, int on);

	void (*UpdateMouse)(_THIS);

	SDL_Overlay *(*CreateYUVOverlay)(_THIS, int width, int height,
	                                 Uint32 format, SDL_Surface *display);

	int (*SetColors)(_THIS, int firstcolor, int ncolors,
			 SDL_Color *colors);

	void (*UpdateRects)(_THIS, int numrects, SDL_Rect *rects);

	void (*VideoQuit)(_THIS);

	
	

	
	SDL_VideoInfo info;

	
	SDL_PixelFormat* displayformatalphapixel;
	
	
	int (*AllocHWSurface)(_THIS, SDL_Surface *surface);

	int (*CheckHWBlit)(_THIS, SDL_Surface *src, SDL_Surface *dst);

	
	int (*FillHWRect)(_THIS, SDL_Surface *dst, SDL_Rect *rect, Uint32 color);

	
	int (*SetHWColorKey)(_THIS, SDL_Surface *surface, Uint32 key);

	
	int (*SetHWAlpha)(_THIS, SDL_Surface *surface, Uint8 value);

	
	int (*LockHWSurface)(_THIS, SDL_Surface *surface);
	void (*UnlockHWSurface)(_THIS, SDL_Surface *surface);

	
	int (*FlipHWSurface)(_THIS, SDL_Surface *surface);

	
	void (*FreeHWSurface)(_THIS, SDL_Surface *surface);

	
	

	Uint16 *gamma;

	
	int (*SetGamma)(_THIS, float red, float green, float blue);

	
	int (*GetGamma)(_THIS, float *red, float *green, float *blue);

	
	int (*SetGammaRamp)(_THIS, Uint16 *ramp);

	
	int (*GetGammaRamp)(_THIS, Uint16 *ramp);

	
	

	
	int (*GL_LoadLibrary)(_THIS, const char *path);

	
	void* (*GL_GetProcAddress)(_THIS, const char *proc);

	
	int (*GL_GetAttribute)(_THIS, SDL_GLattr attrib, int* value);

	
	int (*GL_MakeCurrent)(_THIS);

	
	void (*GL_SwapBuffers)(_THIS);

  	
#if SDL_VIDEO_OPENGL
#if !defined(__WIN32__)
#define WINAPI
#endif
#define SDL_PROC(ret,func,params) ret (WINAPI *func) params;
#include "SDL_glfuncs.h"
#undef SDL_PROC

	
	GLuint texture;
#endif
	int is_32bit;
 
	
	

	
	void (*SetCaption)(_THIS, const char *title, const char *icon);

	
	void (*SetIcon)(_THIS, SDL_Surface *icon, Uint8 *mask);

	int (*IconifyWindow)(_THIS);

	
	SDL_GrabMode (*GrabInput)(_THIS, SDL_GrabMode mode);

	
	int (*GetWMInfo)(_THIS, SDL_SysWMinfo *info);

	
	void (*SetWindowPos)(_THIS, int x, int y);

	
	void (*GetWindowPos)(_THIS, int *px, int *py);

	
	int (*IsWindowVisible)(_THIS, int recenter);

	
	int (*GetMonitorDPI)(_THIS, int *xdpi, int *ydpi);

	
	int (*GetMonitorRect)(_THIS, SDL_Rect* rect);

	
	

	void (*FreeWMCursor)(_THIS, WMcursor *cursor);

	
	WMcursor *(*CreateWMCursor)(_THIS,
		Uint8 *data, Uint8 *mask, int w, int h, int hot_x, int hot_y);

	
	int (*ShowWMCursor)(_THIS, WMcursor *cursor);

	void (*WarpWMCursor)(_THIS, Uint16 x, Uint16 y);

	
	void (*MoveWMCursor)(_THIS, int x, int y);

	void (*CheckMouseMode)(_THIS);

	
	

	
	void (*InitOSKeymap)(_THIS);

	
	void (*PumpEvents)(_THIS);

	
	
	SDL_Surface *screen;
	SDL_Surface *shadow;
	SDL_Surface *visible;
        SDL_Palette *physpal;	
        SDL_Color *gammacols;	
	char *wm_title;
	char *wm_icon;
	int offset_x;
	int offset_y;
	SDL_GrabMode input_grab;

	
	int handles_any_size;	

	
	
	struct {
		int red_size;
		int green_size;
		int blue_size;
		int alpha_size;
		int depth_size;
		int buffer_size;
		int stencil_size;
		int double_buffer;
		int accum_red_size;
		int accum_green_size;
		int accum_blue_size;
		int accum_alpha_size;
		int stereo;
		int multisamplebuffers;
		int multisamplesamples;
		int accelerated;
		int swap_control;
		int driver_loaded;
		char driver_path[256];
		void* dll_handle;
	} gl_config;

	
	
	struct SDL_PrivateVideoData *hidden;
	struct SDL_PrivateGLData *gl_data;

	
	
	void (*free)(_THIS);
};
#undef _THIS

typedef struct VideoBootStrap {
	const char *name;
	const char *desc;
	int (*available)(void);
	SDL_VideoDevice *(*create)(int devindex);
} VideoBootStrap;

#if SDL_VIDEO_DRIVER_QUARTZ
extern VideoBootStrap QZ_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_X11
extern VideoBootStrap X11_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_DGA
extern VideoBootStrap DGA_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_NANOX
extern VideoBootStrap NX_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_IPOD
extern VideoBootStrap iPod_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_QTOPIA
extern VideoBootStrap Qtopia_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_WSCONS
extern VideoBootStrap WSCONS_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_FBCON
extern VideoBootStrap FBCON_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_DIRECTFB
extern VideoBootStrap DirectFB_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_PS2GS
extern VideoBootStrap PS2GS_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_PS3
extern VideoBootStrap PS3_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_GGI
extern VideoBootStrap GGI_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_VGL
extern VideoBootStrap VGL_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_SVGALIB
extern VideoBootStrap SVGALIB_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_GAPI
extern VideoBootStrap GAPI_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_WINDIB
extern VideoBootStrap WINDIB_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_DDRAW
extern VideoBootStrap DIRECTX_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_BWINDOW
extern VideoBootStrap BWINDOW_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_TOOLBOX
extern VideoBootStrap TOOLBOX_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_DRAWSPROCKET
extern VideoBootStrap DSp_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_PHOTON
extern VideoBootStrap ph_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_EPOC
extern VideoBootStrap EPOC_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_XBIOS
extern VideoBootStrap XBIOS_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_GEM
extern VideoBootStrap GEM_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_PICOGUI
extern VideoBootStrap PG_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_DC
extern VideoBootStrap DC_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_NDS
extern VideoBootStrap NDS_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_RISCOS
extern VideoBootStrap RISCOS_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_OS2FS
extern VideoBootStrap OS2FSLib_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_AALIB
extern VideoBootStrap AALIB_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_CACA
extern VideoBootStrap CACA_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_DUMMY
extern VideoBootStrap DUMMY_bootstrap;
#endif

extern SDL_VideoDevice *current_video;

#define SDL_VideoSurface	(current_video->screen)
#define SDL_ShadowSurface	(current_video->shadow)
#define SDL_PublicSurface	(current_video->visible)

#endif 

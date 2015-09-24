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


#include <unistd.h>
#include <sys/ioctl.h>
#ifdef MTRR_SUPPORT
#include <asm/mtrr.h>
#include <sys/fcntl.h>
#endif

#include "SDL_endian.h"
#include "SDL_timer.h"
#include "SDL_thread.h"
#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"
#include "SDL_x11video.h"
#include "SDL_x11wm_c.h"
#include "SDL_x11mouse_c.h"
#include "SDL_x11events_c.h"
#include "SDL_x11modes_c.h"
#include "SDL_x11image_c.h"
#include "SDL_x11yuv_c.h"
#include "SDL_x11gl_c.h"
#include "SDL_x11gamma_c.h"
#include "../blank_cursor.h"

#ifdef X_HAVE_UTF8_STRING
#include <locale.h>
#endif

static int X11_VideoInit(_THIS, SDL_PixelFormat *vformat);
static SDL_Surface *X11_SetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags);
static int X11_ToggleFullScreen(_THIS, int on);
static void X11_UpdateMouse(_THIS);
static int X11_SetColors(_THIS, int firstcolor, int ncolors,
			 SDL_Color *colors);
static int X11_SetGammaRamp(_THIS, Uint16 *ramp);
static void X11_VideoQuit(_THIS);

int  X11_wmXAdjust;
int  X11_wmYAdjust;


static int X11_Available(void)
{
	Display *display = NULL;
	if ( SDL_X11_LoadSymbols() ) {
		display = XOpenDisplay(NULL);
		if ( display != NULL ) {
			XCloseDisplay(display);
		}
		SDL_X11_UnloadSymbols();
	}
	return(display != NULL);
}

static void X11_DeleteDevice(SDL_VideoDevice *device)
{
	if ( device ) {
		if ( device->hidden ) {
			SDL_free(device->hidden);
		}
		if ( device->gl_data ) {
			SDL_free(device->gl_data);
		}
		SDL_free(device);
		SDL_X11_UnloadSymbols();
	}
}

static SDL_VideoDevice *X11_CreateDevice(int devindex)
{
	SDL_VideoDevice *device = NULL;

	if ( SDL_X11_LoadSymbols() ) {
		
		device = (SDL_VideoDevice *)SDL_malloc(sizeof(SDL_VideoDevice));
		if ( device ) {
			SDL_memset(device, 0, (sizeof *device));
			device->hidden = (struct SDL_PrivateVideoData *)
					SDL_malloc((sizeof *device->hidden));
			device->gl_data = (struct SDL_PrivateGLData *)
					SDL_malloc((sizeof *device->gl_data));
		}
		if ( (device == NULL) || (device->hidden == NULL) ||
		                         (device->gl_data == NULL) ) {
			SDL_OutOfMemory();
			X11_DeleteDevice(device); 
			return(0);
		}
		SDL_memset(device->hidden, 0, (sizeof *device->hidden));
		SDL_memset(device->gl_data, 0, (sizeof *device->gl_data));

#if SDL_VIDEO_OPENGL_GLX
		device->gl_data->swap_interval = -1;
#endif

		
		device->handles_any_size = 1;

		
		device->VideoInit = X11_VideoInit;
		device->ListModes = X11_ListModes;
		device->SetVideoMode = X11_SetVideoMode;
		device->ToggleFullScreen = X11_ToggleFullScreen;
		device->UpdateMouse = X11_UpdateMouse;
#if SDL_VIDEO_DRIVER_X11_XV
		device->CreateYUVOverlay = X11_CreateYUVOverlay;
#endif
		device->SetColors = X11_SetColors;
		device->UpdateRects = NULL;
		device->VideoQuit = X11_VideoQuit;
		device->AllocHWSurface = X11_AllocHWSurface;
		device->CheckHWBlit = NULL;
		device->FillHWRect = NULL;
		device->SetHWColorKey = NULL;
		device->SetHWAlpha = NULL;
		device->LockHWSurface = X11_LockHWSurface;
		device->UnlockHWSurface = X11_UnlockHWSurface;
		device->FlipHWSurface = X11_FlipHWSurface;
		device->FreeHWSurface = X11_FreeHWSurface;
		device->SetGamma = X11_SetVidModeGamma;
		device->GetGamma = X11_GetVidModeGamma;
		device->SetGammaRamp = X11_SetGammaRamp;
		device->GetGammaRamp = NULL;
#if SDL_VIDEO_OPENGL_GLX
		device->GL_LoadLibrary = X11_GL_LoadLibrary;
		device->GL_GetProcAddress = X11_GL_GetProcAddress;
		device->GL_GetAttribute = X11_GL_GetAttribute;
		device->GL_MakeCurrent = X11_GL_MakeCurrent;
		device->GL_SwapBuffers = X11_GL_SwapBuffers;
#endif
		device->SetCaption = X11_SetCaption;
		device->SetIcon = X11_SetIcon;
		device->IconifyWindow = X11_IconifyWindow;
		device->GrabInput = X11_GrabInput;
		device->GetWindowPos = X11_GetWindowPos;
		device->SetWindowPos = X11_SetWindowPos;
		device->IsWindowVisible = X11_IsWindowVisible;
		device->GetMonitorDPI = X11_GetMonitorDPI;
		device->GetMonitorRect = X11_GetMonitorRect;
		device->GetWMInfo = X11_GetWMInfo;
		device->FreeWMCursor = X11_FreeWMCursor;
		device->CreateWMCursor = X11_CreateWMCursor;
		device->ShowWMCursor = X11_ShowWMCursor;
		device->WarpWMCursor = X11_WarpWMCursor;
		device->CheckMouseMode = X11_CheckMouseMode;
		device->InitOSKeymap = X11_InitOSKeymap;
		device->PumpEvents = X11_PumpEvents;

		device->free = X11_DeleteDevice;
	}

	return device;
}

VideoBootStrap X11_bootstrap = {
	"x11", "X Window System",
	X11_Available, X11_CreateDevice
};

static int (*X_handler)(Display *, XErrorEvent *) = NULL;
static int x_errhandler(Display *d, XErrorEvent *e)
{
#if SDL_VIDEO_DRIVER_X11_VIDMODE
	extern int vm_error;
#endif
#if SDL_VIDEO_DRIVER_X11_DGAMOUSE
	extern int dga_error;
#endif

#if SDL_VIDEO_DRIVER_X11_VIDMODE
	
        if ( (vm_error >= 0) &&
	     (((e->error_code == BadRequest)&&(e->request_code == vm_error)) ||
	      ((e->error_code > vm_error) &&
	       (e->error_code <= (vm_error+XF86VidModeNumberErrors)))) ) {
#ifdef X11_DEBUG
{ char errmsg[1024];
  XGetErrorText(d, e->error_code, errmsg, sizeof(errmsg));
printf("VidMode error: %s\n", errmsg);
}
#endif
        	return(0);
        }
#endif 

#if SDL_VIDEO_DRIVER_X11_DGAMOUSE
	
        if ( (dga_error >= 0) &&
	     ((e->error_code > dga_error) &&
	      (e->error_code <= (dga_error+XF86DGANumberErrors))) ) {
#ifdef X11_DEBUG
{ char errmsg[1024];
  XGetErrorText(d, e->error_code, errmsg, sizeof(errmsg));
printf("DGA error: %s\n", errmsg);
}
#endif
        	return(0);
        }
#endif 

	return(X_handler(d,e));
}

static int (*XIO_handler)(Display *) = NULL;
static int xio_errhandler(Display *d)
{
	

	
	if ( SDL_VideoSurface && current_video->hidden->Ximage ) {
		SDL_VideoSurface->pixels = NULL;
	}
	current_video->hidden->X11_Display = NULL;

	
	return(XIO_handler(d));
}

static int (*Xext_handler)(Display *, _Xconst char *, _Xconst char *) = NULL;
static int xext_errhandler(Display *d, _Xconst char *ext, _Xconst char *reason)
{
#ifdef X11_DEBUG
	printf("Xext error inside SDL (may be harmless):\n");
	printf("  Extension \"%s\" %s on display \"%s\".\n",
	       ext, reason, XDisplayString(d));
#endif

	if (SDL_strcmp(reason, "missing") == 0) {
		return 0;
	}

	
	return Xext_handler(d, ext, reason);
}

static char *get_classname(char *classname, int maxlen)
{
	char *spot;
#if defined(__LINUX__) || defined(__FREEBSD__)
	char procfile[1024];
	char linkfile[1024];
	int linksize;
#endif

	
	spot = SDL_getenv("SDL_VIDEO_X11_WMCLASS");
	if ( spot ) {
		SDL_strlcpy(classname, spot, maxlen);
		return classname;
	}

	
#if defined(__LINUX__) || defined(__FREEBSD__)
#if defined(__LINUX__)
	SDL_snprintf(procfile, SDL_arraysize(procfile), "/proc/%d/exe", getpid());
#elif defined(__FREEBSD__)
	SDL_snprintf(procfile, SDL_arraysize(procfile), "/proc/%d/file", getpid());
#else
#error Where can we find the executable name?
#endif
	linksize = readlink(procfile, linkfile, sizeof(linkfile)-1);
	if ( linksize > 0 ) {
		linkfile[linksize] = '\0';
		spot = SDL_strrchr(linkfile, '/');
		if ( spot ) {
			SDL_strlcpy(classname, spot+1, maxlen);
		} else {
			SDL_strlcpy(classname, linkfile, maxlen);
		}
		return classname;
	}
#endif 

	
	SDL_strlcpy(classname, "SDL_App", maxlen);
	return classname;
}

static void create_aux_windows(_THIS)
{
    int x = 0, y = 0;
    char classname[1024];
    XSetWindowAttributes xattr;
    XWMHints *hints;
    unsigned long app_event_mask;
    int def_vis = (SDL_Visual == DefaultVisual(SDL_Display, SDL_Screen));

    
    WM_DELETE_WINDOW = XInternAtom(SDL_Display, "WM_DELETE_WINDOW", False);

    
    if ( SDL_windowid ) {
	FSwindow = 0;
	WMwindow = SDL_strtol(SDL_windowid, NULL, 0);
        return;
    }

    if(FSwindow)
	XDestroyWindow(SDL_Display, FSwindow);

#if SDL_VIDEO_DRIVER_X11_XINERAMA
    if ( use_xinerama ) {
        x = xinerama_info.x_org;
        y = xinerama_info.y_org;
    }
#endif
    xattr.override_redirect = True;
    xattr.background_pixel = def_vis ? BlackPixel(SDL_Display, SDL_Screen) : 0;
    xattr.border_pixel = 0;
    xattr.colormap = SDL_XColorMap;

    FSwindow = XCreateWindow(SDL_Display, SDL_Root,
                             x + X11_wmXAdjust,
                             y + X11_wmYAdjust,
                             32, 32, 0,
			     this->hidden->depth, InputOutput, SDL_Visual,
			     CWOverrideRedirect | CWBackPixel | CWBorderPixel
			     | CWColormap,
			     &xattr);

    XSelectInput(SDL_Display, FSwindow, StructureNotifyMask);

    
    {
	XEvent ev;
	long mask;

	SDL_memset(&ev, 0, sizeof(ev));
	ev.xclient.type = ClientMessage;
	ev.xclient.window = SDL_Root;
	ev.xclient.message_type = XInternAtom(SDL_Display,
					      "KWM_KEEP_ON_TOP", False);
	ev.xclient.format = 32;
	ev.xclient.data.l[0] = FSwindow;
	ev.xclient.data.l[1] = CurrentTime;
	mask = SubstructureRedirectMask;
	XSendEvent(SDL_Display, SDL_Root, False, mask, &ev);
    }

    hints = NULL;
    if(WMwindow) {
	
	hints = XGetWMHints(SDL_Display, WMwindow);
	XDestroyWindow(SDL_Display, WMwindow);
    }

    
    
    WMwindow = XCreateWindow(SDL_Display, SDL_Root,
                             x, y, 32, 32, 0,
			     this->hidden->depth, InputOutput, SDL_Visual,
			     CWBackPixel | CWBorderPixel | CWColormap,
			     &xattr);

    
    if(!hints) {
	hints = XAllocWMHints();
	hints->input = True;
	hints->flags = InputHint;
    }
    XSetWMHints(SDL_Display, WMwindow, hints);
    XFree(hints);
    X11_SetCaptionNoLock(this, this->wm_title, this->wm_icon);

    app_event_mask = FocusChangeMask | KeyPressMask | KeyReleaseMask
	| PropertyChangeMask | StructureNotifyMask | KeymapStateMask;
    XSelectInput(SDL_Display, WMwindow, app_event_mask);

    
    get_classname(classname, sizeof(classname));
    {
	XClassHint *classhints;
	classhints = XAllocClassHint();
	if(classhints != NULL) {
	    classhints->res_name = classname;
	    classhints->res_class = classname;
	    XSetClassHint(SDL_Display, WMwindow, classhints);
	    XFree(classhints);
	}
    }

	{
		pid_t pid = getpid();
		char hostname[256];

		if (pid > 0 && gethostname(hostname, sizeof(hostname)) > -1) {
			Atom _NET_WM_PID = XInternAtom(SDL_Display, "_NET_WM_PID", False);
			Atom WM_CLIENT_MACHINE = XInternAtom(SDL_Display, "WM_CLIENT_MACHINE", False);
			
			hostname[sizeof(hostname)-1] = '\0';
			XChangeProperty(SDL_Display, WMwindow, _NET_WM_PID, XA_CARDINAL, 32,
					PropModeReplace, (unsigned char *)&pid, 1);
			XChangeProperty(SDL_Display, WMwindow, WM_CLIENT_MACHINE, XA_STRING, 8,
					PropModeReplace, (unsigned char *)hostname, SDL_strlen(hostname));
		}
	}

	

	#ifdef X_HAVE_UTF8_STRING
	if (SDL_X11_HAVE_UTF8) {
		
		if (SDL_IM != NULL && SDL_Display != XDisplayOfIM(SDL_IM)) {
			SDL_SetError("display has changed while an IM is kept");
			if (SDL_IC) {
				XUnsetICFocus(SDL_IC);
				XDestroyIC(SDL_IC);
				SDL_IC = NULL;
			}
			XCloseIM(SDL_IM);
			SDL_IM = NULL;
		}

		
		if (SDL_IM == NULL) {
			char *old_locale = NULL, *old_modifiers = NULL;
			const char *p;
			size_t n;

			p = setlocale(LC_ALL, NULL);
			if ( p ) {
				n = SDL_strlen(p)+1;
				old_locale = SDL_stack_alloc(char, n);
				if ( old_locale ) {
					SDL_strlcpy(old_locale, p, n);
				}
			}
			p = XSetLocaleModifiers(NULL);
			if ( p ) {
				n = SDL_strlen(p)+1;
				old_modifiers = SDL_stack_alloc(char, n);
				if ( old_modifiers ) {
					SDL_strlcpy(old_modifiers, p, n);
				}
			}

			setlocale(LC_ALL, "");
			XSetLocaleModifiers("");
			SDL_IM = XOpenIM(SDL_Display, NULL, classname, classname);

			if ( old_locale ) {
				setlocale(LC_ALL, old_locale);
				SDL_stack_free(old_locale);
			}
			if ( old_modifiers ) {
				XSetLocaleModifiers(old_modifiers);
				SDL_stack_free(old_modifiers);
			}
		}

		
		if (SDL_IM == NULL) {
			SDL_SetError("no input method could be opened");
		} else {
			if (SDL_IC != NULL) {
				
			    XUnsetICFocus(SDL_IC);
			    XDestroyIC(SDL_IC);
			}
			SDL_IC = pXCreateIC(SDL_IM,
					XNClientWindow, WMwindow,
					XNFocusWindow, WMwindow,
					XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
					XNResourceName, classname,
					XNResourceClass, classname,
					NULL);

			if (SDL_IC == NULL) {
				SDL_SetError("no input context could be created");
				XCloseIM(SDL_IM);
				SDL_IM = NULL;
			} else {
				unsigned long mask = 0;
				char *ret = pXGetICValues(SDL_IC, XNFilterEvents, &mask, NULL);
				if (ret != NULL) {
					XUnsetICFocus(SDL_IC);
					XDestroyIC(SDL_IC);
					SDL_IC = NULL;
					SDL_SetError("no input context could be created");
					XCloseIM(SDL_IM);
					SDL_IM = NULL;
				} else {
					XSelectInput(SDL_Display, WMwindow, app_event_mask | mask);
					XSetICFocus(SDL_IC);
				}
			}
		}
	}
	#endif

	
	XSetWMProtocols(SDL_Display, WMwindow, &WM_DELETE_WINDOW, 1);
}

static int X11_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
	const char *env;
	char *display;
	int i;

	
	display = NULL;		

	if ( (SDL_strncmp(XDisplayName(display), ":", 1) == 0) ||
	     (SDL_strncmp(XDisplayName(display), "unix:", 5) == 0) ) {
		local_X11 = 1;
	} else {
		local_X11 = 0;
	}
	SDL_Display = XOpenDisplay(display);
#if defined(__osf__) && defined(SDL_VIDEO_DRIVER_X11_DYNAMIC)
	if ( SDL_Display == NULL ) {
		SDL_Delay(1000);
		SDL_Display = XOpenDisplay(display);
	}
#endif
	if ( SDL_Display == NULL ) {
		SDL_SetError("Couldn't open X11 display");
		return(-1);
	}
#ifdef X11_DEBUG
	XSynchronize(SDL_Display, True);
#endif

	GFX_Display = XOpenDisplay(display);
	if ( GFX_Display == NULL ) {
		XCloseDisplay(SDL_Display);
		SDL_Display = NULL;
		SDL_SetError("Couldn't open X11 display");
		return(-1);
	}

	
	X_handler = XSetErrorHandler(x_errhandler);

	
	XIO_handler = XSetIOErrorHandler(xio_errhandler);

	
	Xext_handler = XSetExtensionErrorHandler(xext_errhandler);

	
	SDL_Screen = DefaultScreen(SDL_Display);

#ifndef NO_SHARED_MEMORY
	
	use_mitshm = 0;
	if ( local_X11 ) {
		use_mitshm = XShmQueryExtension(SDL_Display);
	}
#endif 

	
	if(X11_GetVideoModes(this) < 0) {
		XCloseDisplay(GFX_Display);
		GFX_Display = NULL;
		XCloseDisplay(SDL_Display);
		SDL_Display = NULL;
	    return -1;
	}

	
	this->info.current_w = DisplayWidth(SDL_Display, SDL_Screen);
	this->info.current_h = DisplayHeight(SDL_Display, SDL_Screen);

	SDL_DisplayColormap = DefaultColormap(SDL_Display, SDL_Screen);
	for(i = 0; i < this->hidden->nvisuals; i++)
	    if(this->hidden->visuals[i].depth == DefaultDepth(SDL_Display,
							      SDL_Screen))
		break;
	if(i == this->hidden->nvisuals) {
	    
	    i = 0;
	}
	SDL_Visual = this->hidden->visuals[i].visual;
	if ( SDL_Visual == DefaultVisual(SDL_Display, SDL_Screen) ) {
	    SDL_XColorMap = SDL_DisplayColormap;
	} else {
	    SDL_XColorMap = XCreateColormap(SDL_Display, SDL_Root,
					    SDL_Visual, AllocNone);
	}
	this->hidden->depth = this->hidden->visuals[i].depth;
	vformat->BitsPerPixel = this->hidden->visuals[i].bpp;
	if ( vformat->BitsPerPixel > 8 ) {
		vformat->Rmask = SDL_Visual->red_mask;
	  	vformat->Gmask = SDL_Visual->green_mask;
	  	vformat->Bmask = SDL_Visual->blue_mask;
	}
	if ( this->hidden->depth == 32 ) {
		vformat->Amask = (0xFFFFFFFF & ~(vformat->Rmask|vformat->Gmask|vformat->Bmask));
	}
	X11_SaveVidModeGamma(this);

	
	env = SDL_getenv("SDL_VIDEO_ALLOW_SCREENSAVER");
	if ( env ) {
		allow_screensaver = SDL_atoi(env);
	} else {
#ifdef SDL_VIDEO_DISABLE_SCREENSAVER
		allow_screensaver = 0;
#else
		allow_screensaver = 1;
#endif
	}

	
	SDL_windowid = SDL_getenv("SDL_WINDOWID");

	
	create_aux_windows(this);

	
	SDL_BlankCursor = this->CreateWMCursor(this, blank_cdata, blank_cmask,
					BLANK_CWIDTH, BLANK_CHEIGHT,
						BLANK_CHOTX, BLANK_CHOTY);

	
	this->info.wm_available = 1;

	
	XFlush(SDL_Display);
	return(0);
}

static void X11_DestroyWindow(_THIS, SDL_Surface *screen)
{
	
	if ( screen ) {
		screen->flags &= ~(SDL_OPENGL|SDL_OPENGLBLIT);
	}
	X11_GL_Shutdown(this);

	if ( ! SDL_windowid ) {
		
		if ( WMwindow ) {
			XUnmapWindow(SDL_Display, WMwindow);
		}
		if ( screen && (screen->flags & SDL_FULLSCREEN) ) {
			screen->flags &= ~SDL_FULLSCREEN;
			X11_LeaveFullScreen(this);
		}

		
		if ( SDL_Window ) {
			XDestroyWindow(SDL_Display, SDL_Window);
		}

		
		if ( SDL_XPixels ) {
			int numcolors;
			unsigned long pixel;
			numcolors = SDL_Visual->map_entries;
			for ( pixel=0; pixel<numcolors; ++pixel ) {
				while ( SDL_XPixels[pixel] > 0 ) {
					XFreeColors(GFX_Display,
						SDL_DisplayColormap,&pixel,1,0);
					--SDL_XPixels[pixel];
				}
			}
			SDL_free(SDL_XPixels);
			SDL_XPixels = NULL;
		} 

		
		if ( SDL_GC ) {
			XFreeGC(SDL_Display, SDL_GC);
			SDL_GC = 0;
		}
	}
}

static SDL_bool X11_WindowPosition(_THIS, int *x, int *y, int w, int h)
{
	const char *window = SDL_getenv("SDL_VIDEO_WINDOW_POS");
	const char *center = SDL_getenv("SDL_VIDEO_CENTERED");
	if ( window ) {
		if ( SDL_sscanf(window, "%d,%d", x, y) == 2 ) {
			return SDL_TRUE;
		}
		if ( SDL_strcmp(window, "center") == 0 ) {
			center = window;
		}
	}
	if ( center ) {
		*x = (DisplayWidth(SDL_Display, SDL_Screen) - w)/2;
		*y = (DisplayHeight(SDL_Display, SDL_Screen) - h)/2;
		return SDL_TRUE;
	}
	return SDL_FALSE;
}

static void X11_SetSizeHints(_THIS, int w, int h, Uint32 flags)
{
	XSizeHints *hints;

	hints = XAllocSizeHints();
	if ( hints ) {
		if (!(flags & SDL_RESIZABLE)) {
			hints->min_width = hints->max_width = w;
			hints->min_height = hints->max_height = h;
			hints->flags = PMaxSize | PMinSize;
		}
		if ( flags & SDL_FULLSCREEN ) {
			hints->x = 0;
			hints->y = 0;
			hints->flags |= USPosition;
		} else
		
		if ( X11_WindowPosition(this, &hints->x, &hints->y, w, h) ) {
			hints->flags |= USPosition;

			XSetWMNormalHints(SDL_Display, WMwindow, hints);

			XMoveWindow(SDL_Display, WMwindow, hints->x, hints->y);

			
			XSync(SDL_Display, True);
		}
		XSetWMNormalHints(SDL_Display, WMwindow, hints);
		XFree(hints);
	}

	
	if ( flags & SDL_NOFRAME ) {
		SDL_bool set;
		Atom WM_HINTS;

		
		set = SDL_FALSE;

		
		WM_HINTS = XInternAtom(SDL_Display, "_MOTIF_WM_HINTS", True);
		if ( WM_HINTS != None ) {
			
			struct {
				unsigned long flags;
				unsigned long functions;
				unsigned long decorations;
				long input_mode;
				unsigned long status;
			} MWMHints = { (1L << 1), 0, 0, 0, 0 };

			XChangeProperty(SDL_Display, WMwindow,
			                WM_HINTS, WM_HINTS, 32,
			                PropModeReplace,
					(unsigned char *)&MWMHints,
					sizeof(MWMHints)/sizeof(long));
			set = SDL_TRUE;
		}
		
		WM_HINTS = XInternAtom(SDL_Display, "KWM_WIN_DECORATION", True);
		if ( WM_HINTS != None ) {
			long KWMHints = 0;

			XChangeProperty(SDL_Display, WMwindow,
			                WM_HINTS, WM_HINTS, 32,
			                PropModeReplace,
					(unsigned char *)&KWMHints,
					sizeof(KWMHints)/sizeof(long));
			set = SDL_TRUE;
		}
		
		WM_HINTS = XInternAtom(SDL_Display, "_WIN_HINTS", True);
		if ( WM_HINTS != None ) {
			long GNOMEHints = 0;

			XChangeProperty(SDL_Display, WMwindow,
			                WM_HINTS, WM_HINTS, 32,
			                PropModeReplace,
					(unsigned char *)&GNOMEHints,
					sizeof(GNOMEHints)/sizeof(long));
			set = SDL_TRUE;
		}
		
		if ( ! set ) {
			XSetTransientForHint(SDL_Display, WMwindow, SDL_Root);
		}
	} else {
		SDL_bool set;
		Atom WM_HINTS;

		
		set = SDL_FALSE;

		
		WM_HINTS = XInternAtom(SDL_Display, "_MOTIF_WM_HINTS", True);
		if ( WM_HINTS != None ) {
			XDeleteProperty(SDL_Display, WMwindow, WM_HINTS);
			set = SDL_TRUE;
		}
		
		WM_HINTS = XInternAtom(SDL_Display, "KWM_WIN_DECORATION", True);
		if ( WM_HINTS != None ) {
			XDeleteProperty(SDL_Display, WMwindow, WM_HINTS);
			set = SDL_TRUE;
		}
		
		WM_HINTS = XInternAtom(SDL_Display, "_WIN_HINTS", True);
		if ( WM_HINTS != None ) {
			XDeleteProperty(SDL_Display, WMwindow, WM_HINTS);
			set = SDL_TRUE;
		}
		
		if ( ! set ) {
			XDeleteProperty(SDL_Display, WMwindow, XA_WM_TRANSIENT_FOR);
		}
	}
}

static int X11_CreateWindow(_THIS, SDL_Surface *screen,
			    int w, int h, int bpp, Uint32 flags)
{
	int i, depth;
	Visual *vis;
	int vis_change;
	Uint32 Amask;

	
	if ( SDL_Window ) {
		X11_DestroyWindow(this, screen);
		switch_waiting = 0; 
	}

	
	if ( SDL_windowid ) {
		SDL_Window = SDL_strtol(SDL_windowid, NULL, 0);
	} else {
		SDL_Window = 0;
	}

	
	if ( flags & SDL_OPENGL ) {
		XVisualInfo *vi;

		vi = X11_GL_GetVisual(this);
		if( !vi ) {
			return -1;
		}
		vis = vi->visual;
		depth = vi->depth;
	} else if ( SDL_windowid ) {
		XWindowAttributes a;

		XGetWindowAttributes(SDL_Display, SDL_Window, &a);
		vis = a.visual;
		depth = a.depth;
	} else {
		for ( i = 0; i < this->hidden->nvisuals; i++ ) {
			if ( this->hidden->visuals[i].bpp == bpp )
				break;
		}
		if ( i == this->hidden->nvisuals ) {
			SDL_SetError("No matching visual for requested depth");
			return -1;	
		}
		vis = this->hidden->visuals[i].visual;
		depth = this->hidden->visuals[i].depth;
	}
#ifdef X11_DEBUG
        printf("Choosing %s visual at %d bpp - %d colormap entries\n", vis->class == PseudoColor ? "PseudoColor" : (vis->class == TrueColor ? "TrueColor" : (vis->class == DirectColor ? "DirectColor" : "Unknown")), depth, vis->map_entries);
#endif
	vis_change = (vis != SDL_Visual);
	SDL_Visual = vis;
	this->hidden->depth = depth;

	
	if ( this->hidden->depth == 32 ) {
		Amask = (0xFFFFFFFF & ~(vis->red_mask|vis->green_mask|vis->blue_mask));
	} else {
		Amask = 0;
	}
	if ( ! SDL_ReallocFormat(screen, bpp,
			vis->red_mask, vis->green_mask, vis->blue_mask, Amask) ) {
		return -1;
	}

	
	if ( SDL_XColorMap != SDL_DisplayColormap ) {
		XFreeColormap(SDL_Display, SDL_XColorMap);
	}
	if ( SDL_Visual->class == PseudoColor ) {
	    int ncolors;

	    
	    ncolors = SDL_Visual->map_entries;
	    SDL_XPixels = SDL_malloc(ncolors * sizeof(int));
	    if(SDL_XPixels == NULL) {
		SDL_OutOfMemory();
		return -1;
	    }
	    SDL_memset(SDL_XPixels, 0, ncolors * sizeof(*SDL_XPixels));

	    
	    if ( SDL_Visual != DefaultVisual(SDL_Display, SDL_Screen) ) {
		flags |= SDL_HWPALETTE;
	    }
	    if ( flags & SDL_HWPALETTE ) {
		screen->flags |= SDL_HWPALETTE;
		SDL_XColorMap = XCreateColormap(SDL_Display, SDL_Root,
		                                SDL_Visual, AllocAll);
	    } else {
		SDL_XColorMap = SDL_DisplayColormap;
	    }
	} else if ( SDL_Visual->class == DirectColor ) {

	    
	    SDL_XColorMap = XCreateColormap(SDL_Display, SDL_Root,
		                            SDL_Visual, AllocAll);
            XSync(SDL_Display, False);

	    
	    SDL_GetGammaRamp(0, 0, 0);
	    this->screen = screen;
	    X11_SetGammaRamp(this, this->gamma);
	    this->screen = NULL;
	} else {
	    
	    SDL_XColorMap = XCreateColormap(SDL_Display, SDL_Root,
	                                    SDL_Visual, AllocNone);
	}

	
	if ( vis_change )
	    create_aux_windows(this);

	if(screen->flags & SDL_HWPALETTE) {
	    XSetWindowBackground(SDL_Display, FSwindow, 0);
	    XClearWindow(SDL_Display, FSwindow);
	}

	
	if( !SDL_windowid ) {
	        X11_SetSizeHints(this, w, h, flags);
		window_w = w;
		window_h = h;
		XResizeWindow(SDL_Display, WMwindow, w, h);
	}

	
	if ( !SDL_windowid ) {
		if ( flags & SDL_OPENGL ) {
			if ( X11_GL_CreateWindow(this, w, h) < 0 ) {
				return(-1);
			}
		} else {
			XSetWindowAttributes swa;

			swa.background_pixel = 0;
			swa.border_pixel = 0;
			swa.colormap = SDL_XColorMap;
			SDL_Window = XCreateWindow(SDL_Display, WMwindow,
		                           	0, 0, w, h, 0, depth,
		                           	InputOutput, SDL_Visual,
		                           	CWBackPixel | CWBorderPixel
		                           	| CWColormap, &swa);
		}
		
		XSelectInput(SDL_Display, SDL_Window,
					( EnterWindowMask | LeaveWindowMask
					| ButtonPressMask | ButtonReleaseMask
					| PointerMotionMask | ExposureMask ));
	}
	
	if ( flags & SDL_OPENGL ) {
		if ( X11_GL_CreateContext(this) < 0 ) {
			return(-1);
		} else {
			screen->flags |= SDL_OPENGL;
		}
	} else {
		XGCValues gcv;

		gcv.graphics_exposures = False;
		SDL_GC = XCreateGC(SDL_Display, SDL_Window,
		                   GCGraphicsExposures, &gcv);
		if ( ! SDL_GC ) {
			SDL_SetError("Couldn't create graphics context");
			return(-1);
		}
	}

	
	if ( ! (flags & SDL_OPENGL) ) {
		XSetWindowColormap(SDL_Display, SDL_Window, SDL_XColorMap);
		if( !SDL_windowid ) {
		    XSetWindowColormap(SDL_Display, FSwindow, SDL_XColorMap);
		    XSetWindowColormap(SDL_Display, WMwindow, SDL_XColorMap);
		}
	}

#if 0 
	if ( SDL_getenv("SDL_VIDEO_X11_BACKINGSTORE") )
#endif
	
	{
		Screen *xscreen;
		XSetWindowAttributes a;

		xscreen = ScreenOfDisplay(SDL_Display, SDL_Screen);
		a.backing_store = DoesBackingStore(xscreen);
		if ( a.backing_store != NotUseful ) {
			XChangeWindowAttributes(SDL_Display, SDL_Window,
			                        CWBackingStore, &a);
		}
	}

	
	if ( ! SDL_windowid ) {
		XMapWindow(SDL_Display, SDL_Window);
		XMapWindow(SDL_Display, WMwindow);
		X11_WaitMapped(this, WMwindow);
		if ( flags & SDL_FULLSCREEN ) {
			screen->flags |= SDL_FULLSCREEN;
			X11_EnterFullScreen(this);
		} else {
			screen->flags &= ~SDL_FULLSCREEN;
		}
	}
	
	return(0);
}

static int X11_ResizeWindow(_THIS,
			SDL_Surface *screen, int w, int h, Uint32 flags)
{
	if ( ! SDL_windowid ) {
		
		X11_SetSizeHints(this, w, h, flags);
		window_w = w;
		window_h = h;
		XResizeWindow(SDL_Display, WMwindow, w, h);

		
		if ( flags & SDL_FULLSCREEN ) {
			if ( screen->flags & SDL_FULLSCREEN ) {
				X11_ResizeFullScreen(this);
			} else {
				screen->flags |= SDL_FULLSCREEN;
				X11_EnterFullScreen(this);
			}
		} else {
			if ( screen->flags & SDL_FULLSCREEN ) {
				screen->flags &= ~SDL_FULLSCREEN;
				X11_LeaveFullScreen(this);
			}
		}
		XResizeWindow(SDL_Display, SDL_Window, w, h);
	}
	return(0);
}

SDL_Surface *X11_SetVideoMode(_THIS, SDL_Surface *current,
				int width, int height, int bpp, Uint32 flags)
{
	Uint32 saved_flags;

	
	SDL_Lock_EventThread();

	
	if ( flags & SDL_FULLSCREEN ) {
		
		if ( SDL_windowid ) {
			flags &= ~SDL_FULLSCREEN;
		}
	}

	
	XSync(GFX_Display, False);

	
	saved_flags = current->flags;
	if ( (SDL_Window) && ((saved_flags&SDL_OPENGL) == (flags&SDL_OPENGL))
	      && (bpp == current->format->BitsPerPixel)
          && ((saved_flags&SDL_NOFRAME) == (flags&SDL_NOFRAME)) ) {
		if (X11_ResizeWindow(this, current, width, height, flags) < 0) {
			current = NULL;
			goto done;
		}
		X11_PendingConfigureNotifyWidth = width;
		X11_PendingConfigureNotifyHeight = height;
	} else {
		if (X11_CreateWindow(this,current,width,height,bpp,flags) < 0) {
			current = NULL;
			goto done;
		}
	}

	
	X11_SetKeyboardState(SDL_Display, NULL);

	
	if ( !current->w && !current->h ) {
		Uint8 *keys = SDL_GetKeyState(NULL);
		int i;
		for ( i = 0; i < SDLK_LAST; ++i ) {
			switch (i) {
			    case SDLK_NUMLOCK:
			    case SDLK_CAPSLOCK:
			    case SDLK_LCTRL:
			    case SDLK_RCTRL:
			    case SDLK_LSHIFT:
			    case SDLK_RSHIFT:
			    case SDLK_LALT:
			    case SDLK_RALT:
			    case SDLK_LMETA:
			    case SDLK_RMETA:
			    case SDLK_MODE:
				break;
			    default:
				keys[i] = SDL_RELEASED;
				break;
			}
		}
	}

	
	if ( ((current->w != width) || (current->h != height)) ||
             ((saved_flags&SDL_OPENGL) != (flags&SDL_OPENGL)) ) {
		current->w = width;
		current->h = height;
		current->pitch = SDL_CalculatePitch(current);
		if (X11_ResizeImage(this, current, flags) < 0) {
			current = NULL;
			goto done;
		}
	}

	
	current->flags &= ~(SDL_RESIZABLE|SDL_NOFRAME);
	current->flags |= (flags&(SDL_RESIZABLE|SDL_NOFRAME));

  done:
	
	XSync(SDL_Display, False);
	SDL_Unlock_EventThread();

	
	return(current);
}

static int X11_ToggleFullScreen(_THIS, int on)
{
	Uint32 event_thread;

	
	if ( SDL_windowid ) {
		return(0);
	}

	
	event_thread = SDL_EventThreadID();
	if ( event_thread && (SDL_ThreadID() == event_thread) ) {
		event_thread = 0;
	}
	if ( event_thread ) {
		SDL_Lock_EventThread();
	}
	if ( on ) {
		this->screen->flags |= SDL_FULLSCREEN;
		X11_EnterFullScreen(this);
	} else {
		this->screen->flags &= ~SDL_FULLSCREEN;
		X11_LeaveFullScreen(this);
	}
	X11_RefreshDisplay(this);
	if ( event_thread ) {
		SDL_Unlock_EventThread();
	}
	SDL_ResetKeyboard();
	return(1);
}

static void X11_UpdateMouse(_THIS)
{
	Window u1; int u2;
	Window current_win;
	int x, y;
	unsigned int mask;

	
	SDL_Lock_EventThread();
	if ( XQueryPointer(SDL_Display, SDL_Window, &u1, &current_win,
	                   &u2, &u2, &x, &y, &mask) ) {
		if ( (x >= 0) && (x < SDL_VideoSurface->w) &&
		     (y >= 0) && (y < SDL_VideoSurface->h) ) {
			SDL_PrivateAppActive(1, SDL_APPMOUSEFOCUS);
			SDL_PrivateMouseMotion(0, 0, x, y);
		} else {
			SDL_PrivateAppActive(0, SDL_APPMOUSEFOCUS);
		}
	}
	SDL_Unlock_EventThread();
}

#define COLOUR_FACTOR 3
#define LIGHT_FACTOR 1
#define COLOUR_DIST(r1, g1, b1, r2, g2, b2)				\
	(COLOUR_FACTOR * (abs(r1 - r2) + abs(g1 - g2) + abs(b1 - b2))	\
	 + LIGHT_FACTOR * abs(r1 + g1 + b1 - (r2 + g2 + b2)))

static void allocate_nearest(_THIS, SDL_Color *colors,
			     SDL_Color *want, int nwant)
{
	XColor all[256];
	int i;
	for(i = 0; i < 256; i++)
		all[i].pixel = i;
	XQueryColors(GFX_Display, SDL_XColorMap, all, 256);

	for(i = 0; i < nwant; i++) {
		XColor *c;
		int j;
		int best = 0;
		int mindist = 0x7fffffff;
		int ri = want[i].r;
		int gi = want[i].g;
		int bi = want[i].b;
		for(j = 0; j < 256; j++) {
			int rj, gj, bj, d2;
			if(!all[j].flags)
				continue;	
			rj = all[j].red >> 8;
			gj = all[j].green >> 8;
			bj = all[j].blue >> 8;
			d2 = COLOUR_DIST(ri, gi, bi, rj, gj, bj);
			if(d2 < mindist) {
				mindist = d2;
				best = j;
			}
		}
		if(SDL_XPixels[best])
			continue; 
		c = all + best;
		if(XAllocColor(GFX_Display, SDL_XColorMap, c)) {
			
			colors[c->pixel].r = c->red >> 8;
			colors[c->pixel].g = c->green >> 8;
			colors[c->pixel].b = c->blue >> 8;
			++SDL_XPixels[c->pixel];
		} else {
			c->flags = 0;
			i--;
		}
	}
}

int X11_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors)
{
	int nrej = 0;

	
	if ( SDL_XPixels == NULL ) {
		return(0);
	}
	if ( (this->screen->flags & SDL_HWPALETTE) == SDL_HWPALETTE ) {
	        
	        XColor  *xcmap;
		int i;
	        xcmap = SDL_stack_alloc(XColor, ncolors);
		if(xcmap == NULL)
		        return 0;
		for ( i=0; i<ncolors; ++i ) {
			xcmap[i].pixel = i + firstcolor;
			xcmap[i].red   = (colors[i].r<<8)|colors[i].r;
			xcmap[i].green = (colors[i].g<<8)|colors[i].g;
			xcmap[i].blue  = (colors[i].b<<8)|colors[i].b;
			xcmap[i].flags = (DoRed|DoGreen|DoBlue);
		}
		XStoreColors(GFX_Display, SDL_XColorMap, xcmap, ncolors);
		XSync(GFX_Display, False);
		SDL_stack_free(xcmap);
	} else {
		SDL_Color *want, *reject;
	        unsigned long *freelist;
		int i;
		int nfree = 0;
		int nc = this->screen->format->palette->ncolors;
	        colors = this->screen->format->palette->colors;
		freelist = SDL_stack_alloc(unsigned long, nc);
		
	        for(i = 0; i < ncolors; i++) {
		        int pixel = firstcolor + i;
		        while(SDL_XPixels[pixel]) {
			        freelist[nfree++] = pixel;
				--SDL_XPixels[pixel];
			}
		}
		XFreeColors(GFX_Display, SDL_XColorMap, freelist, nfree, 0);
		SDL_stack_free(freelist);

		want = SDL_stack_alloc(SDL_Color, ncolors);
		reject = SDL_stack_alloc(SDL_Color, ncolors);
		SDL_memcpy(want, colors + firstcolor, ncolors * sizeof(SDL_Color));
		SDL_memset(colors + firstcolor, 0, ncolors * sizeof(SDL_Color));

		
		for(i = 0; i < ncolors; i++) {
		        XColor col;
			col.red = want[i].r << 8;
			col.green = want[i].g << 8;
			col.blue = want[i].b << 8;
			col.flags = DoRed | DoGreen | DoBlue;
			if(XAllocColor(GFX_Display, SDL_XColorMap, &col)) {
			        colors[col.pixel].r = col.red >> 8;
				colors[col.pixel].g = col.green >> 8;
				colors[col.pixel].b = col.blue >> 8;
				++SDL_XPixels[col.pixel];
			} else {
				reject[nrej++] = want[i];
			}
		}
		if(nrej)
			allocate_nearest(this, colors, reject, nrej);
		SDL_stack_free(reject);
		SDL_stack_free(want);
	}
	return nrej == 0;
}

int X11_SetGammaRamp(_THIS, Uint16 *ramp)
{
	int i, ncolors;
	XColor xcmap[256];

	
	if ( SDL_Visual->class != DirectColor ) {
	    SDL_SetError("Gamma correction not supported on this visual");
	    return(-1);
	}

	
	ncolors = SDL_Visual->map_entries;
	for ( i=0; i<ncolors; ++i ) {
		Uint8 c = (256 * i / ncolors);
		xcmap[i].pixel = SDL_MapRGB(this->screen->format, c, c, c);
		xcmap[i].red   = ramp[0*256+c];
		xcmap[i].green = ramp[1*256+c];
		xcmap[i].blue  = ramp[2*256+c];
		xcmap[i].flags = (DoRed|DoGreen|DoBlue);
	}
	XStoreColors(GFX_Display, SDL_XColorMap, xcmap, ncolors);
	XSync(GFX_Display, False);
	return(0);
}

void X11_VideoQuit(_THIS)
{
	
	
	if ( SDL_Display != NULL ) {
		
		XSync(GFX_Display, False);

		
		#ifdef X_HAVE_UTF8_STRING
		if (SDL_IC != NULL) {
			XUnsetICFocus(SDL_IC);
			XDestroyIC(SDL_IC);
			SDL_IC = NULL;
		}
		if (SDL_IM != NULL) {
			XCloseIM(SDL_IM);
			SDL_IM = NULL;
		}
		#endif

		
		X11_DestroyImage(this, this->screen);
		X11_DestroyWindow(this, this->screen);
		X11_FreeVideoModes(this);
		if ( SDL_XColorMap != SDL_DisplayColormap ) {
			XFreeColormap(SDL_Display, SDL_XColorMap);
		}
		if ( SDL_iconcolors ) {
			unsigned long pixel;
			Colormap dcmap = DefaultColormap(SDL_Display,
							 SDL_Screen);
			for(pixel = 0; pixel < 256; ++pixel) {
				while(SDL_iconcolors[pixel] > 0) {
					XFreeColors(GFX_Display,
						    dcmap, &pixel, 1, 0);
					--SDL_iconcolors[pixel];
				}
			}
			SDL_free(SDL_iconcolors);
			SDL_iconcolors = NULL;
		} 

		
		if ( SDL_GetAppState() & SDL_APPACTIVE ) {
			X11_SwapVidModeGamma(this);
		}

		
		if ( SDL_BlankCursor != NULL ) {
			this->FreeWMCursor(this, SDL_BlankCursor);
			SDL_BlankCursor = NULL;
		}

		
		if ( GFX_Display != NULL ) {
			XCloseDisplay(GFX_Display);
			GFX_Display = NULL;
		}

		
		XCloseDisplay(SDL_Display);
		SDL_Display = NULL;

		
		if ( XIO_handler ) {
			XSetIOErrorHandler(XIO_handler);
		}
		if ( X_handler ) {
			XSetErrorHandler(X_handler);
		}

		
		X11_GL_UnloadLibrary(this);
	}
	if ( this->screen && (this->screen->flags & SDL_HWSURFACE) ) {
		
		this->screen->pixels = NULL;
	}

#if SDL_VIDEO_DRIVER_X11_XME
    XiGMiscDestroy();
#endif
}


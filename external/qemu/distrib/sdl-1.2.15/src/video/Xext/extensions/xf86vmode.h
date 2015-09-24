/*

Copyright 1995  Kaleb S. KEITHLEY

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL Kaleb S. KEITHLEY BE LIABLE FOR ANY CLAIM, DAMAGES 
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of Kaleb S. KEITHLEY 
shall not be used in advertising or otherwise to promote the sale, use 
or other dealings in this Software without prior written authorization
from Kaleb S. KEITHLEY

*/


#ifndef _XF86VIDMODE_H_
#define _XF86VIDMODE_H_

#include <X11/Xfuncproto.h>
#include <X11/Xmd.h>
#include "SDL_name.h"

#define X_XF86VidModeQueryVersion	0
#define X_XF86VidModeGetModeLine	1
#define X_XF86VidModeModModeLine	2
#define X_XF86VidModeSwitchMode		3
#define X_XF86VidModeGetMonitor		4
#define X_XF86VidModeLockModeSwitch	5
#define X_XF86VidModeGetAllModeLines	6
#define X_XF86VidModeAddModeLine	7
#define X_XF86VidModeDeleteModeLine	8
#define X_XF86VidModeValidateModeLine	9
#define X_XF86VidModeSwitchToMode	10
#define X_XF86VidModeGetViewPort	11
#define X_XF86VidModeSetViewPort	12
#define X_XF86VidModeGetDotClocks	13
#define X_XF86VidModeSetClientVersion	14
#define X_XF86VidModeSetGamma		15
#define X_XF86VidModeGetGamma		16
#define X_XF86VidModeGetGammaRamp	17
#define X_XF86VidModeSetGammaRamp	18
#define X_XF86VidModeGetGammaRampSize	19

#define CLKFLAG_PROGRAMABLE		1

#ifdef XF86VIDMODE_EVENTS
#define XF86VidModeNotify		0
#define XF86VidModeNumberEvents		(XF86VidModeNotify + 1)

#define XF86VidModeNotifyMask		0x00000001

#define XF86VidModeNonEvent		0
#define XF86VidModeModeChange		1
#else
#define XF86VidModeNumberEvents		0
#endif

#define XF86VidModeBadClock		0
#define XF86VidModeBadHTimings		1
#define XF86VidModeBadVTimings		2
#define XF86VidModeModeUnsuitable	3
#define XF86VidModeExtensionDisabled	4
#define XF86VidModeClientNotLocal	5
#define XF86VidModeZoomLocked		6
#define XF86VidModeNumberErrors		(XF86VidModeZoomLocked + 1)

#ifndef _XF86VIDMODE_SERVER_

typedef struct {
    unsigned short	hdisplay;
    unsigned short	hsyncstart;
    unsigned short	hsyncend;
    unsigned short	htotal;
    unsigned short	hskew;
    unsigned short	vdisplay;
    unsigned short	vsyncstart;
    unsigned short	vsyncend;
    unsigned short	vtotal;
    unsigned int	flags;
    int			privsize;
#if defined(__cplusplus) || defined(c_plusplus)
    
    INT32		*c_private;
#else
    INT32		*private;
#endif
} SDL_NAME(XF86VidModeModeLine);

typedef struct {
    unsigned int	dotclock;
    unsigned short	hdisplay;
    unsigned short	hsyncstart;
    unsigned short	hsyncend;
    unsigned short	htotal;
    unsigned short	hskew;
    unsigned short	vdisplay;
    unsigned short	vsyncstart;
    unsigned short	vsyncend;
    unsigned short	vtotal;
    unsigned int	flags;
    int			privsize;
#if defined(__cplusplus) || defined(c_plusplus)
    
    INT32		*c_private;
#else
    INT32		*private;
#endif
} SDL_NAME(XF86VidModeModeInfo);

typedef struct {
    float		hi;
    float		lo;
} SDL_NAME(XF86VidModeSyncRange);

typedef struct {
    char*			vendor;
    char*			model;
    float			EMPTY;
    unsigned char		nhsync;
    SDL_NAME(XF86VidModeSyncRange)*	hsync;
    unsigned char		nvsync;
    SDL_NAME(XF86VidModeSyncRange)*	vsync;
} SDL_NAME(XF86VidModeMonitor);
    
typedef struct {
    int type;			
    unsigned long serial;	
    Bool send_event;		
    Display *display;		
    Window root;		
    int state;			
    int kind;			
    Bool forced;		
    Time time;			
} SDL_NAME(XF86VidModeNotifyEvent);

typedef struct {
    float red;			
    float green;		
    float blue;			
} SDL_NAME(XF86VidModeGamma);


#define SDL_XF86VidModeSelectNextMode(disp, scr) \
	SDL_NAME(XF86VidModeSwitchMode)(disp, scr, 1)
#define SDL_XF86VidModeSelectPrevMode(disp, scr) \
	SDL_NAME(XF86VidModeSwitchMode)(disp, scr, -1)

_XFUNCPROTOBEGIN

Bool SDL_NAME(XF86VidModeQueryVersion)(
    Display*		,
    int*		,
    int*		
);

Bool SDL_NAME(XF86VidModeQueryExtension)(
    Display*		,
    int*		,
    int*		
);

Bool SDL_NAME(XF86VidModeSetClientVersion)(
    Display*		
);

Bool SDL_NAME(XF86VidModeGetModeLine)(
    Display*			,
    int				,
    int*			,
    SDL_NAME(XF86VidModeModeLine)*	
);

Bool SDL_NAME(XF86VidModeGetAllModeLines)(
    Display*			,
    int				,
    int*			,
    SDL_NAME(XF86VidModeModeInfo)***	
);

Bool SDL_NAME(XF86VidModeAddModeLine)(
    Display*			,
    int				,
    SDL_NAME(XF86VidModeModeInfo)*	,
    SDL_NAME(XF86VidModeModeInfo)*	
);

Bool SDL_NAME(XF86VidModeDeleteModeLine)(
    Display*			,
    int				,
    SDL_NAME(XF86VidModeModeInfo)*	
);

Bool SDL_NAME(XF86VidModeModModeLine)(
    Display*			,
    int				,
    SDL_NAME(XF86VidModeModeLine)*	
);

Status SDL_NAME(XF86VidModeValidateModeLine)(
    Display*			,
    int				,
    SDL_NAME(XF86VidModeModeInfo)*	
);

Bool SDL_NAME(XF86VidModeSwitchMode)(
    Display*		,
    int			,
    int			
);

Bool SDL_NAME(XF86VidModeSwitchToMode)(
    Display*			,
    int				,
    SDL_NAME(XF86VidModeModeInfo)*	
);

Bool SDL_NAME(XF86VidModeLockModeSwitch)(
    Display*		,
    int			,
    int			
);

Bool SDL_NAME(XF86VidModeGetMonitor)(
    Display*		,
    int			,
    SDL_NAME(XF86VidModeMonitor)*	
);

Bool SDL_NAME(XF86VidModeGetViewPort)(
    Display*		,
    int			,
    int*		,
    int*		
);

Bool SDL_NAME(XF86VidModeSetViewPort)(
    Display*		,
    int			,
    int			,
    int			
);

Bool SDL_NAME(XF86VidModeGetDotClocks)(
    Display*		,
    int			,
    int*		,
    int*		,
    int*		,
    int**		
);

Bool SDL_NAME(XF86VidModeGetGamma)(
    Display*			,
    int				,
    SDL_NAME(XF86VidModeGamma)*		
);

Bool SDL_NAME(XF86VidModeSetGamma)(
    Display*			,
    int				,
    SDL_NAME(XF86VidModeGamma)*		
);

Bool SDL_NAME(XF86VidModeSetGammaRamp)(
    Display*                    ,
    int                         ,
    int				, 
    unsigned short*             ,
    unsigned short*             ,
    unsigned short*             
);

Bool SDL_NAME(XF86VidModeGetGammaRamp)(
    Display*                    ,
    int                         ,
    int                         ,
    unsigned short*             ,
    unsigned short*             ,
    unsigned short*             
);

Bool SDL_NAME(XF86VidModeGetGammaRampSize)(
    Display*                    ,
    int                         ,
    int*                        
);


_XFUNCPROTOEND

#endif

#endif

/*

Copyright (c) 1995  Jon Tombs
Copyright (c) 1995  XFree86 Inc

*/


#ifndef _XF86DGA1_H_
#define _XF86DGA1_H_

#include <X11/Xfuncproto.h>
#include "SDL_name.h"

#define X_XF86DGAQueryVersion		0
#define X_XF86DGAGetVideoLL		1
#define X_XF86DGADirectVideo		2
#define X_XF86DGAGetViewPortSize	3
#define X_XF86DGASetViewPort		4
#define X_XF86DGAGetVidPage		5
#define X_XF86DGASetVidPage		6
#define X_XF86DGAInstallColormap	7
#define X_XF86DGAQueryDirectVideo	8
#define X_XF86DGAViewPortChanged	9

#define XF86DGADirectPresent		0x0001
#define XF86DGADirectGraphics		0x0002
#define XF86DGADirectMouse		0x0004
#define XF86DGADirectKeyb		0x0008
#define XF86DGAHasColormap		0x0100
#define XF86DGADirectColormap		0x0200




#ifndef _XF86DGA_SERVER_

_XFUNCPROTOBEGIN

Bool SDL_NAME(XF86DGAQueryVersion)(
#if NeedFunctionPrototypes
    Display*		,
    int*		,
    int*		
#endif
);

Bool SDL_NAME(XF86DGAQueryExtension)(
#if NeedFunctionPrototypes
    Display*		,
    int*		,
    int*		
#endif
);

Status SDL_NAME(XF86DGAGetVideoLL)(
#if NeedFunctionPrototypes
    Display*			,
    int				,
    int *			,
    int *			,
    int *			,
    int *			 
#endif
);

Status SDL_NAME(XF86DGAGetVideo)(
#if NeedFunctionPrototypes
    Display*			,
    int				,
    char **			,
    int *			,
    int *			,
    int *			
#endif
);

Status SDL_NAME(XF86DGADirectVideo)(
#if NeedFunctionPrototypes
    Display*			,
    int				,
    int 			
#endif
);

Status SDL_NAME(XF86DGADirectVideoLL)(
#if NeedFunctionPrototypes
    Display*			,
    int				,
    int 			
#endif
);

Status SDL_NAME(XF86DGAGetViewPortSize)(
#if NeedFunctionPrototypes
    Display*			,
    int				,
    int *			,
    int *			
#endif
);

Status SDL_NAME(XF86DGASetViewPort)(
#if NeedFunctionPrototypes
    Display*			,
    int				,
    int x			,
    int y			
#endif
);

Status SDL_NAME(XF86DGAGetVidPage)(
#if NeedFunctionPrototypes
    Display*			,
    int				,
    int *			
#endif
);

Status SDL_NAME(XF86DGASetVidPage)(
#if NeedFunctionPrototypes
    Display*			,
    int				,
    int				
#endif
);

Status SDL_NAME(XF86DGAInstallColormap)(
#if NeedFunctionPrototypes
    Display*			,
    int				,
    Colormap			
#endif
);

int SDL_NAME(XF86DGAForkApp)(
#if NeedFunctionPrototypes
    int screen
#endif
);

Status SDL_NAME(XF86DGAQueryDirectVideo)(
#if NeedFunctionPrototypes
    Display *		,
    int			,
    int *		
#endif
);

Bool SDL_NAME(XF86DGAViewPortChanged)(
#if NeedFunctionPrototypes
    Display *		,
    int			,
    int			
#endif
);


_XFUNCPROTOEND

#endif 

#endif 

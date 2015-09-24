/*
 * $Xorg: extutil.h,v 1.4 2001/02/09 02:03:24 xorgcvs Exp $
 *
Copyright 1989, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.
 *
 * Author:  Jim Fulton, MIT The Open Group
 * 
 *                     Xlib Extension-Writing Utilities
 *
 * This package contains utilities for writing the client API for various
 * protocol extensions.  THESE INTERFACES ARE NOT PART OF THE X STANDARD AND
 * ARE SUBJECT TO CHANGE!
 */

#ifndef _EXTUTIL_H_
#define _EXTUTIL_H_

#include "SDL_stdinc.h"		

#include "./Xext.h"

typedef struct _XExtDisplayInfo {
    struct _XExtDisplayInfo *next;	
    Display *display;			
    XExtCodes *codes;			
    XPointer data;			
} XExtDisplayInfo;

typedef struct _XExtensionInfo {
    XExtDisplayInfo *head;		
    XExtDisplayInfo *cur;		
    int ndisplays;			
} XExtensionInfo;

typedef struct _XExtensionHooks {
    int (*create_gc)(
#if NeedNestedPrototypes
	      Display*			,
	      GC			,
	      XExtCodes*		
#endif
);
    int (*copy_gc)(
#if NeedNestedPrototypes
	      Display*			,
              GC			,
              XExtCodes*		
#endif
);
    int (*flush_gc)(
#if NeedNestedPrototypes
	      Display*			,
              GC			,
              XExtCodes*		
#endif
);
    int (*free_gc)(
#if NeedNestedPrototypes
	      Display*			,
              GC			,
              XExtCodes*		
#endif
);
    int (*create_font)(
#if NeedNestedPrototypes
	      Display*			,
              XFontStruct*		,
              XExtCodes*		
#endif
);
    int (*free_font)(
#if NeedNestedPrototypes
	      Display*			,
              XFontStruct*		,
              XExtCodes*		
#endif
);
    int (*close_display)(
#if NeedNestedPrototypes
	      Display*			,
              XExtCodes*		
#endif
);
    Bool (*wire_to_event)(
#if NeedNestedPrototypes
	       Display*			,
               XEvent*			,
               xEvent*			
#endif
);
    Status (*event_to_wire)(
#if NeedNestedPrototypes
	      Display*			,
              XEvent*			,
              xEvent*			
#endif
);
    int (*error)(
#if NeedNestedPrototypes
	      Display*			,
              xError*			,
              XExtCodes*		,
              int*			
#endif
);
    char *(*error_string)(
#if NeedNestedPrototypes
	        Display*		,
                int			,
                XExtCodes*		,
                char*			,
                int			
#endif
);
} XExtensionHooks;

extern XExtensionInfo *XextCreateExtension(
#if NeedFunctionPrototypes
    void
#endif
);
extern void XextDestroyExtension(
#if NeedFunctionPrototypes
    XExtensionInfo*	
#endif
);
extern XExtDisplayInfo *XextAddDisplay(
#if NeedFunctionPrototypes
    XExtensionInfo*	,
    Display*		,
    char*		,
    XExtensionHooks*	,
    int			,
    XPointer		
#endif
);
extern int XextRemoveDisplay(
#if NeedFunctionPrototypes
    XExtensionInfo*	,
    Display*		
#endif
);
extern XExtDisplayInfo *XextFindDisplay(
#if NeedFunctionPrototypes
    XExtensionInfo*	,
    Display*		
#endif
);

#define XextHasExtension(i) ((i) && ((i)->codes))
#define XextCheckExtension(dpy,i,name,val) \
  if (!XextHasExtension(i)) { XMissingExtension (dpy, name); return val; }
#define XextSimpleCheckExtension(dpy,i,name) \
  if (!XextHasExtension(i)) { XMissingExtension (dpy, name); return; }


#define XEXT_GENERATE_FIND_DISPLAY(proc,extinfo,extname,hooks,nev,data) \
XExtDisplayInfo *proc (Display *dpy) \
{ \
    XExtDisplayInfo *dpyinfo; \
    if (!extinfo) { if (!(extinfo = XextCreateExtension())) return NULL; } \
    if (!(dpyinfo = XextFindDisplay (extinfo, dpy))) \
      dpyinfo = XextAddDisplay (extinfo,dpy,extname,hooks,nev,data); \
    return dpyinfo; \
}

#define XEXT_FIND_DISPLAY_PROTO(proc) \
	XExtDisplayInfo *proc(Display *dpy)

#define XEXT_GENERATE_CLOSE_DISPLAY(proc,extinfo) \
int proc (Display *dpy, XExtCodes *codes) \
{ \
    return XextRemoveDisplay (extinfo, dpy); \
}

#define XEXT_CLOSE_DISPLAY_PROTO(proc) \
	int proc(Display *dpy, XExtCodes *codes)

#define XEXT_GENERATE_ERROR_STRING(proc,extname,nerr,errl) \
char *proc (Display *dpy, int code, XExtCodes *codes, char *buf, int n) \
{  \
    code -= codes->first_error;  \
    if (code >= 0 && code < nerr) { \
	char tmp[256]; \
	SDL_snprintf (tmp, SDL_arraysize(tmp), "%s.%d", extname, code); \
	XGetErrorDatabaseText (dpy, "XProtoError", tmp, errl[code], buf, n); \
	return buf; \
    } \
    return (char *)0; \
}

#define XEXT_ERROR_STRING_PROTO(proc) \
	char *proc(Display *dpy, int code, XExtCodes *codes, char *buf, int n)
#endif

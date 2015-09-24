/***********************************************************
Copyright 1991 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

#ifndef XVLIB_H
#define XVLIB_H

#include <X11/Xfuncproto.h>
#include "Xv.h"
#include "SDL_name.h"

typedef struct {
  int numerator;
  int denominator;
} SDL_NAME(XvRational);

typedef struct {
  int flags;	
  int min_value;
  int max_value;
  char *name;
} SDL_NAME(XvAttribute);

typedef struct {
  XvEncodingID encoding_id;
  char *name;
  unsigned long width;
  unsigned long height;
  SDL_NAME(XvRational) rate;
  unsigned long num_encodings;
} SDL_NAME(XvEncodingInfo);

typedef struct {
  char depth;
  unsigned long visual_id;
} SDL_NAME(XvFormat);

typedef struct {
  XvPortID base_id;
  unsigned long num_ports;
  char type;
  char *name;
  unsigned long num_formats;
  SDL_NAME(XvFormat) *formats;
  unsigned long num_adaptors;
} SDL_NAME(XvAdaptorInfo);

typedef struct {
  int type;
  unsigned long serial;	   
  Bool send_event;	   
  Display *display;	   
  Drawable drawable;       
  unsigned long reason;    
  XvPortID port_id;        
  Time time;		   
} SDL_NAME(XvVideoNotifyEvent);

typedef struct {
  int type;
  unsigned long serial;	   
  Bool send_event;	   
  Display *display;	   
  XvPortID port_id;        
  Time time;		   
  Atom attribute;           
  long value;              
} SDL_NAME(XvPortNotifyEvent);

typedef union {
  int type;
  SDL_NAME(XvVideoNotifyEvent) xvvideo;
  SDL_NAME(XvPortNotifyEvent) xvport;
  long pad[24];
} SDL_NAME(XvEvent);

typedef struct {
  int id;                      
  int type;                    
  int byte_order;              
  char guid[16];               
  int bits_per_pixel;
  int format;                  
  int num_planes;

  
  int depth;
  unsigned int red_mask;       
  unsigned int green_mask;   
  unsigned int blue_mask;   

  
  unsigned int y_sample_bits;
  unsigned int u_sample_bits;
  unsigned int v_sample_bits;   
  unsigned int horz_y_period;
  unsigned int horz_u_period;
  unsigned int horz_v_period;
  unsigned int vert_y_period;
  unsigned int vert_u_period;
  unsigned int vert_v_period;
  char component_order[32];    
  int scanline_order;          
} SDL_NAME(XvImageFormatValues); 

typedef struct {
  int id;
  int width, height;
  int data_size;              
  int num_planes;
  int *pitches;               
  int *offsets;               
  char *data;
  XPointer obdata;     
} SDL_NAME(XvImage);

_XFUNCPROTOBEGIN

extern int SDL_NAME(XvQueryExtension)(
#if NeedFunctionPrototypes
  Display*                 ,
  unsigned int*            ,
  unsigned int*            ,
  unsigned int*            ,
  unsigned int*            , 
  unsigned int*            
#endif
);

extern int SDL_NAME(XvQueryAdaptors)(
#if NeedFunctionPrototypes
  Display*                 ,
  Window                   ,
  unsigned int*            ,
  SDL_NAME(XvAdaptorInfo)**          
#endif
);

extern int SDL_NAME(XvQueryEncodings)(
#if NeedFunctionPrototypes
  Display*                 ,
  XvPortID                 ,
  unsigned int*            ,
  SDL_NAME(XvEncodingInfo)**         
#endif
);

extern int SDL_NAME(XvPutVideo)(
#if NeedFunctionPrototypes
  Display*                 ,
  XvPortID                 ,
  Drawable                 ,
  GC                       ,
  int                      , 
  int                      ,
  unsigned int             , 
  unsigned int             ,
  int                      , 
  int                      ,
  unsigned int             ,
  unsigned int             
#endif
);

extern int SDL_NAME(XvPutStill)(
#if NeedFunctionPrototypes
  Display*                 ,
  XvPortID                 ,
  Drawable                 ,
  GC                       ,
  int                      , 
  int                      ,
  unsigned int             , 
  unsigned int             ,
  int                      , 
  int                      ,
  unsigned int             ,
  unsigned int             
#endif
);

extern int SDL_NAME(XvGetVideo)(
#if NeedFunctionPrototypes
  Display*                 ,
  XvPortID                 ,
  Drawable                 ,
  GC                       ,
  int                      , 
  int                      ,
  unsigned int             , 
  unsigned int             ,
  int                      , 
  int                      ,
  unsigned int             ,
  unsigned int             
#endif
);

extern int SDL_NAME(XvGetStill)(
#if NeedFunctionPrototypes
  Display*                 ,
  XvPortID                 ,
  Drawable                 ,
  GC                       ,
  int                      , 
  int                      ,
  unsigned int             , 
  unsigned int             ,
  int                      , 
  int                      ,
  unsigned int             ,
  unsigned int             
#endif
);

extern int SDL_NAME(XvStopVideo)(
#if NeedFunctionPrototypes
  Display*                ,
  XvPortID                ,
  Drawable                
#endif
);

extern int SDL_NAME(XvGrabPort)(
#if NeedFunctionPrototypes
  Display*                ,
  XvPortID                ,
  Time                    
#endif
);

extern int SDL_NAME(XvUngrabPort)(
#if NeedFunctionPrototypes
  Display*                ,
  XvPortID                ,
  Time                    
#endif
);

extern int SDL_NAME(XvSelectVideoNotify)(
#if NeedFunctionPrototypes
  Display*                ,
  Drawable                ,
  Bool                    
#endif
);

extern int SDL_NAME(XvSelectPortNotify)(
#if NeedFunctionPrototypes
  Display*                ,
  XvPortID                ,
  Bool                    
#endif
);

extern int SDL_NAME(XvSetPortAttribute)(
#if NeedFunctionPrototypes
  Display*                ,
  XvPortID                ,
  Atom                    ,
  int                     
#endif
);

extern int SDL_NAME(XvGetPortAttribute)(
#if NeedFunctionPrototypes
  Display*                ,
  XvPortID                ,
  Atom                    ,
  int*                    
#endif
);

extern int SDL_NAME(XvQueryBestSize)(
#if NeedFunctionPrototypes
  Display*                ,
  XvPortID                ,
  Bool                    ,
  unsigned int            , 
  unsigned int            ,
  unsigned int            , 
  unsigned int            ,
  unsigned int*           , 
  unsigned int*           
#endif
);

extern SDL_NAME(XvAttribute)* SDL_NAME(XvQueryPortAttributes)(
#if NeedFunctionPrototypes
  Display*                ,
  XvPortID                ,
  int*                    
#endif
);


extern void SDL_NAME(XvFreeAdaptorInfo)(
#if NeedFunctionPrototypes
  SDL_NAME(XvAdaptorInfo)*          
#endif
);

extern void SDL_NAME(XvFreeEncodingInfo)(
#if NeedFunctionPrototypes
  SDL_NAME(XvEncodingInfo)*         
#endif
);


extern SDL_NAME(XvImageFormatValues) * SDL_NAME(XvListImageFormats) (
#if NeedFunctionPrototypes
   Display 	*display,
   XvPortID 	port_id,
   int 		*count_return
#endif
);

extern SDL_NAME(XvImage) * SDL_NAME(XvCreateImage) (
#if NeedFunctionPrototypes
   Display *display,
   XvPortID port,
   int id,
   char *data,
   int width, 
   int height 
#endif
);

extern int SDL_NAME(XvPutImage) (
#if NeedFunctionPrototypes
  Display *display,
   XvPortID id,
   Drawable d,
   GC gc,
   SDL_NAME(XvImage) *image,
   int src_x,
   int src_y,
   unsigned int src_w,
   unsigned int src_h,
   int dest_x, 
   int dest_y,
   unsigned int dest_w,
   unsigned int dest_h
#endif
);

extern int SDL_NAME(XvShmPutImage) (
#if NeedFunctionPrototypes
   Display *display,
   XvPortID id,
   Drawable d,
   GC gc,
   SDL_NAME(XvImage) *image,
   int src_x,
   int src_y,
   unsigned int src_w,
   unsigned int src_h,
   int dest_x, 
   int dest_y,
   unsigned int dest_w,
   unsigned int dest_h,
   Bool send_event
#endif
);

#ifdef _XSHM_H_

extern SDL_NAME(XvImage) * SDL_NAME(XvShmCreateImage) (
#if NeedFunctionPrototypes
   Display *display,
   XvPortID port,
   int id,
   char* data,
   int width, 
   int height,
   XShmSegmentInfo *shminfo
#endif
);

#endif


_XFUNCPROTOEND

#endif 

/* Copyright (C) 2007-2008 The Android Open Source Project
**
** This software is licensed under the terms of the GNU General Public
** License version 2, as published by the Free Software Foundation, and
** may be copied, distributed, and modified under those terms.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/
#ifndef _ANDROID_FRAMEBUFFER_H_
#define _ANDROID_FRAMEBUFFER_H_

typedef struct QFrameBuffer   QFrameBuffer;


typedef enum {
    QFRAME_BUFFER_NONE   = 0,
    QFRAME_BUFFER_RGB565 = 1,
    QFRAME_BUFFER_RGBX_8888 = 2,
    QFRAME_BUFFER_MAX          
} QFrameBufferFormat;

struct QFrameBuffer {
    int                 width;        
    int                 height;       
    int                 pitch;        
    int                 bits_per_pixel; 
    int                 bytes_per_pixel;    
    int                 rotation;     
    QFrameBufferFormat  format;
    void*               pixels;       

    int                 phys_width_mm;
    int                 phys_height_mm;

    
    void*               extra;

};

#define  DEFAULT_FRAMEBUFFER_DPI   165


extern int
qframebuffer_init( QFrameBuffer*       qfbuff,
                   int                 width,
                   int                 height,
                   int                 rotation,
                   QFrameBufferFormat  format );

extern void
qframebuffer_set_dpi( QFrameBuffer*   qfbuff,
                      int             x_dpi,
                      int             y_dpi );

extern void
qframebuffer_set_mm( QFrameBuffer*   qfbuff,
                     int             width_mm,
                     int             height_mm );

typedef void (*QFrameBufferUpdateFunc)( void*  opaque, int  x, int  y,
                                                       int  w, int  h );

typedef void (*QFrameBufferRotateFunc)( void*  opaque, int  rotation );

typedef void (*QFrameBufferPollFunc)( void* opaque );

typedef void (*QFrameBufferDoneFunc)  ( void*  opaque );

extern void
qframebuffer_add_client( QFrameBuffer*           qfbuff,
                         void*                   fb_opaque,
                         QFrameBufferUpdateFunc  fb_update,
                         QFrameBufferRotateFunc  fb_rotate,
                         QFrameBufferPollFunc    fb_poll,
                         QFrameBufferDoneFunc    fb_done );

typedef void (*QFrameBufferCheckUpdateFunc)( void*  opaque );

typedef void (*QFrameBufferInvalidateFunc) ( void*  opaque );

typedef void (*QFrameBufferDetachFunc)     ( void*  opaque );

extern void
qframebuffer_set_producer( QFrameBuffer*                qfbuff,
                           void*                        opaque,
                           QFrameBufferCheckUpdateFunc  fb_check,
                           QFrameBufferInvalidateFunc   fb_invalidate,
                           QFrameBufferDetachFunc       fb_detach );

extern void
qframebuffer_update( QFrameBuffer*  qfbuff, int  x, int  y, int  w, int  h );

extern void
qframebuffer_rotate( QFrameBuffer*  qfbuff, int  rotation );

extern void
qframebuffer_poll( QFrameBuffer* qfbuff );

extern void
qframebuffer_done( QFrameBuffer*   qfbuff );


extern void
qframebuffer_check_updates( void );

extern void
qframebuffer_pulse( void );

extern void
qframebuffer_invalidate_all( void );


extern void
qframebuffer_fifo_add( QFrameBuffer*  qfbuff );

extern QFrameBuffer*
qframebuffer_fifo_get( void );


#endif 

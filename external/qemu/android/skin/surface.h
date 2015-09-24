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
#ifndef _ANDROID_SKIN_SURFACE_H
#define _ANDROID_SKIN_SURFACE_H

#include "android/skin/region.h"
#include <stdint.h>

typedef struct SkinSurface  SkinSurface;

extern SkinSurface*  skin_surface_ref( SkinSurface*  surface );

extern void          skin_surface_unrefp( SkinSurface*  *psurface );

typedef void (*SkinSurfaceDoneFunc)( void*  user );

extern void  skin_surface_set_done( SkinSurface*  s, SkinSurfaceDoneFunc  done_func, void*  done_user );



extern SkinSurface*  skin_surface_create_fast( int  w, int  h );

extern SkinSurface*  skin_surface_create_slow( int  w, int  h );

extern SkinSurface*  skin_surface_create_argb32_from(
                            int                  w,
                            int                  h,
                            int                  pitch,
                            uint32_t*            pixels,
                            int                  do_copy );

typedef struct {
    int         w;
    int         h;
    int         pitch;
    uint32_t*   pixels;
} SkinSurfacePixels;

extern int     skin_surface_lock  ( SkinSurface*  s, SkinSurfacePixels  *pix );

extern void    skin_surface_unlock( SkinSurface*  s );

typedef enum {
    SKIN_BLIT_COPY = 0,
    SKIN_BLIT_SRCOVER,
    SKIN_BLIT_DSTOVER,
} SkinBlitOp;


extern void    skin_surface_blit( SkinSurface*  dst,
                                  SkinPos*      dst_pos,
                                  SkinSurface*  src,
                                  SkinRect*     src_rect,
                                  SkinBlitOp    blitop );

extern void    skin_surface_fill( SkinSurface*  dst,
                                  SkinRect*     rect,
                                  uint32_t      argb_premul,
                                  SkinBlitOp    blitop );

#endif 

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
#ifndef _ANDROID_SKIN_IMAGE_H
#define _ANDROID_SKIN_IMAGE_H

#include "android/android.h"
#include <SDL.h>
#include "android/skin/rect.h"


extern SDL_Surface*    sdl_surface_from_argb32( void*  base, int  w, int  h );


typedef struct SkinImage   SkinImage;

typedef struct SkinImageDesc {
    const char*      path;      
    AndroidRotation  rotation;  
    int              blend;     
} SkinImageDesc;

#define  SKIN_BLEND_NONE   0
#define  SKIN_BLEND_HALF   128
#define  SKIN_BLEND_FULL   256

extern SkinImage*    SKIN_IMAGE_NONE;

extern SDL_Surface*  skin_image_surface( SkinImage*  image );
extern int           skin_image_w      ( SkinImage*  image );
extern int           skin_image_h      ( SkinImage*  image );
extern int           skin_image_org_w  ( SkinImage*  image );
extern int           skin_image_org_h  ( SkinImage*  image );

extern SkinImage*    skin_image_find( SkinImageDesc*  desc );

extern SkinImage*    skin_image_find_simple( const char*  path );

extern SkinImage*    skin_image_ref( SkinImage*  image );

extern void          skin_image_unref( SkinImage**  pimage );

extern SkinImage*    skin_image_rotate( SkinImage*  source, SkinRotation  rotation );

extern SkinImage*    skin_image_clone( SkinImage*  source );

extern SkinImage*    skin_image_clone_full( SkinImage*       source,
                                            SkinRotation     rotation,
                                            int              blend );

extern void          skin_image_blend_clone( SkinImage*  clone, SkinImage*  source, int  blend );

#endif 

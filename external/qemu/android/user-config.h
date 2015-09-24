/* Copyright (C) 2009 The Android Open Source Project
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
#ifndef _ANDROID_USER_CONFIG_H
#define _ANDROID_USER_CONFIG_H

#include "android/avd/info.h"
#include <stdint.h>

typedef struct AUserConfig   AUserConfig;

AUserConfig*   auserConfig_new( AvdInfo*  info );

uint64_t       auserConfig_getUUID( AUserConfig*  uconfig );

void           auserConfig_getWindowPos( AUserConfig*  uconfig, int  *pX, int  *pY );

void           auserConfig_setWindowPos( AUserConfig*  uconfig, int  x, int  y );

void           auserConfig_save( AUserConfig*  uconfig );


#endif 

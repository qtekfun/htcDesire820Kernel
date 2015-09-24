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
#ifndef _android_hw_control_h
#define _android_hw_control_h

#include "qemu-common.h"

typedef void  (*AndroidHwLightBrightnessFunc)( void*       opaque,
                                               const char* light,
                                               int         brightness );

typedef struct {
    AndroidHwLightBrightnessFunc  light_brightness;
} AndroidHwControlFuncs;

extern void  android_hw_control_init( void );

extern void  android_hw_control_set( void*                         opaque,
                                      const AndroidHwControlFuncs*  funcs );

#endif 

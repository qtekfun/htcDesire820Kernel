/* Copyright (C) 2008 The Android Open Source Project
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
#ifndef _ANDROID_HELP_H
#define _ANDROID_HELP_H

#include "android/utils/stralloc.h"

#define  BOOT_PROPERTY_MAX_NAME    32
#define  BOOT_PROPERTY_MAX_VALUE   92

extern void  android_help_list_options( stralloc_t*  out );

extern void  android_help_main( stralloc_t*  out );

extern void  android_help_all( stralloc_t*  out );

extern int  android_help_for_option( const char*  option, stralloc_t*  out );

extern int  android_help_for_topic( const char*  topic, stralloc_t*  out );

#endif 

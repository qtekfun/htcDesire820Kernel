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

#ifndef _ANDROID_BOOT_PROPERTIES_H
#define _ANDROID_BOOT_PROPERTIES_H


#define  PROPERTY_MAX_NAME    32
#define  PROPERTY_MAX_VALUE   92

int  boot_property_add( const char*  name, const char*  value );

int  boot_property_add2( const char*  name, int  namelen,
                         const char*  value, int  valuelen );

void  boot_property_init_service( void );

void  boot_property_parse_option( const char*  param );

#endif 

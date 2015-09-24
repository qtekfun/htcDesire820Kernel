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

#ifndef _ANDROID_UTILS_BUFPRINT_H
#define _ANDROID_UTILS_BUFPRINT_H

#include <stdarg.h>


extern char*   vbufprint(char*  buffer, char*  buffend, const char*  fmt, va_list  args );
extern char*   bufprint (char*  buffer, char*  buffend, const char*  fmt, ... );


extern char*  bufprint_app_dir    (char*  buffer, char*  buffend);
extern char*  bufprint_config_path(char*  buffer, char*  buffend);
extern char*  bufprint_config_file(char*  buffer, char*  buffend, const char*  suffix);
extern char*  bufprint_temp_dir   (char*  buffer, char*  buffend);
extern char*  bufprint_temp_file  (char*  buffer, char*  buffend, const char*  suffix);

#endif 

/* Copyright (C) 2007-2009 The Android Open Source Project
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
#ifndef _ANDROID_UTILS_PATH_H
#define _ANDROID_UTILS_PATH_H

#include <android/utils/system.h>
#include <stdint.h>  


#if !defined(_WIN32) && !defined(O_BINARY)
#  define  O_BINARY  0
#endif

#ifdef _WIN32
#  define  PATH_SEP   "\\"
#  define  PATH_SEP_C '\\'
#else
#  define  PATH_SEP   "/"
#  define  PATH_SEP_C '/'
#endif

#ifdef _WIN32
#  undef   MAX_PATH
#  define  MAX_PATH    1024
#  undef   PATH_MAX
#  define  PATH_MAX    MAX_PATH
#else
#  include <limits.h>
#  define  MAX_PATH    PATH_MAX
#endif

extern ABool  path_exists( const char*  path );

extern ABool  path_is_regular( const char*  path );

extern ABool  path_is_dir( const char*  path );

extern ABool  path_is_absolute( const char*  path );

extern ABool  path_can_read( const char*  path );
extern ABool  path_can_write( const char*  path );

extern ABool  path_can_exec( const char* path );

extern APosixStatus   path_mkdir( const char*  path, int  mode );

extern APosixStatus   path_mkdir_if_needed( const char*  path, int  mode );

extern APosixStatus   path_get_size( const char*  path, uint64_t  *psize );

extern char*  path_parent( const char*  path, int  levels );

extern int    path_split( const char*  path, char* *pdirname, char* *pbasename );

extern char*  path_dirname( const char*  path );

extern char*  path_basename( const char*  path );

extern char*  path_search_exec( const char* filename );

extern char*  path_get_absolute( const char* path );


extern APosixStatus   path_empty_file( const char*  path );

extern APosixStatus   path_copy_file( const char*  dest, const char*  source );

extern APosixStatus   path_delete_file( const char*  path );

extern void*          path_load_file( const char*  path, size_t  *pSize );


#endif 

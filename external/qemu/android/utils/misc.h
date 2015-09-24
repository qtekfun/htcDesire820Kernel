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
#ifndef _ANDROID_UTILS_MISC_H
#define _ANDROID_UTILS_MISC_H

#include <stdint.h>


extern void   print_tabular( const char** strings, int  count,
                             const char*  prefix,  int  width );


extern void   buffer_translate_char( char*        buff,
                                     unsigned     buffLen,
                                     const char*  src,
                                     char         fromChar,
                                     char         toChar );

extern void   string_translate_char( char*  str, char from, char to );


extern char*  tempstr_get( int   size );
extern char*  tempstr_format( const char*  fmt, ... );


extern const char*   quote_bytes( const char*  str, int  len );
extern const char*   quote_str( const char*  str );


extern int    hex2int( const uint8_t*  hex, int  len );

extern void   int2hex( uint8_t*  hex, int  len, int  val );


extern int strtoi(const char *nptr, char **endptr, int base);

extern int get_token_value(const char* params, const char* name, char* value, int val_size);

extern int get_token_value_alloc(const char* params, const char* name, char** value);

extern int get_token_value_int(const char* params, const char* name, int* value);

#endif 

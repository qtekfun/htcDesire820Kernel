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
#ifndef ANDROID_UTILS_PANIC_H
#define ANDROID_UTILS_PANIC_H

#include <stdarg.h>

void __attribute__((noreturn)) android_panic ( const char*  fmt, ... );

void __attribute__((noreturn)) android_vpanic( const char*  fmt, va_list  args );

#define  APANIC(...)    android_panic(__VA_ARGS__)

typedef void (*APanicHandlerFunc)(const char*, va_list) __attribute__((noreturn));

#ifdef ACONFIG_UNIT_TEST
void android_panic_registerHandler( APanicHandlerFunc  handler );
#endif 

#endif 

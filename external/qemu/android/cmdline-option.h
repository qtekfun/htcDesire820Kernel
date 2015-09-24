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
#ifndef _ANDROID_OPTION_H
#define _ANDROID_OPTION_H

typedef struct ParamList {
    char*              param;
    struct ParamList*  next;
} ParamList;

typedef struct {
#define OPT_LIST(n,t,d)    ParamList*  n;
#define OPT_PARAM(n,t,d)   char*  n;
#define OPT_FLAG(n,d)      int    n;
#include "android/cmdline-options.h"
} AndroidOptions;


extern int
android_parse_options( int  *pargc, char**  *pargv, AndroidOptions*  opt );

#define  KEYSET_FILE    "default.keyset"

#define  DEFAULT_DEVICE_DPI  165

#endif 

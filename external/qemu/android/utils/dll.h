/* Copyright (C) 2011 The Android Open Source Project
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
#ifndef ANDROID_UTILS_DLL_H
#define ANDROID_UTILS_DLL_H

typedef struct ADynamicLibrary   ADynamicLibrary;

ADynamicLibrary*   adynamicLibrary_open( const char*  libraryName,
                                         char**       pError);

void* adynamicLibrary_findSymbol( ADynamicLibrary*  lib,
                                  const char*       symbolName,
                                  char**            pError);

void  adynamicLibrary_close( ADynamicLibrary*  lib );

#endif 

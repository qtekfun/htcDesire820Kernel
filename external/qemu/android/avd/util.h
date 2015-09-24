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
#ifndef _ANDROID_AVD_UTIL_H
#define _ANDROID_AVD_UTIL_H


char* path_getSdkRoot( char *pFromEnv );

char* path_getRootIniPath( const char*  avdName );

char* path_getAvdTargetArch( const char* avdName );

char* path_getBuildTargetArch( const char* androidOut );

char* path_getBuildTargetAbi( const char* androidOut );

int path_getBuildTargetApiLevel( const char* androidOut );

int path_getAdbdCommunicationMode( const char* androidOut );

#endif 

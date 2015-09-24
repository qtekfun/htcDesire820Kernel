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
#ifndef ANDROID_OPENGLES_H
#define ANDROID_OPENGLES_H

#include <stddef.h>

int android_initOpenglesEmulation(void);

int android_startOpenglesRenderer(int width, int height);

typedef void (*OnPostFunc)(void* context, int width, int height, int ydir,
                           int format, int type, unsigned char* pixels);
void android_setPostCallback(OnPostFunc onPost, void* onPostContext);

void android_getOpenglesHardwareStrings(char* vendor, size_t vendorBufSize,
                                        char* renderer, size_t rendererBufSize,
                                        char* version, size_t versionBufSize);

int android_showOpenglesWindow(void* window, int x, int y, int width, int height, float rotation);

int android_hideOpenglesWindow(void);

void android_redrawOpenglesWindow(void);

void android_stopOpenglesRenderer(void);

extern int  android_gles_fast_pipes;

void android_gles_server_path(char* buff, size_t buffsize);

#endif 

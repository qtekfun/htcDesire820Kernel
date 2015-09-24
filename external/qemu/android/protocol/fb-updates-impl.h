/* Copyright (C) 2010 The Android Open Source Project
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


#ifndef _ANDROID_FRAMEBUFFER_UI_H
#define _ANDROID_FRAMEBUFFER_UI_H

#include "android/looper.h"
#include "android/framebuffer.h"
#include "android/looper.h"
#include "android/async-utils.h"

int fbUpdatesImpl_create(SockAddress* console_socket,
                         const char* protocol,
                         QFrameBuffer* fb,
                         Looper* looper);

void fbUpdatesImpl_destroy(void);

#endif 

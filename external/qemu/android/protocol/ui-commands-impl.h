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

#ifndef _ANDROID_PROTOCOL_UI_COMMANDS_IMPL_H
#define _ANDROID_PROTOCOL_UI_COMMANDS_IMPL_H

#include "sockets.h"
#include "android/looper.h"
#include "android/protocol/ui-commands.h"


extern int uiCmdImpl_create(SockAddress* console_socket, Looper* looper);

extern void uiCmdImpl_destroy();

#endif 

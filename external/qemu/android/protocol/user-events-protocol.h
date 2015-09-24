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

#ifndef _ANDROID_PROTOCOL_USER_EVENTS_H
#define _ANDROID_PROTOCOL_USER_EVENTS_H


#include "android/globals.h"

#define AUSER_EVENT_MOUSE     0
#define AUSER_EVENT_KEYCODE   1
#define AUSER_EVENT_GENERIC   2

typedef struct UserEventHeader {
    
    uint8_t event_type;
} UserEventHeader;

typedef struct UserEventMouse {
    int         dx;
    int         dy;
    int         dz;
    unsigned    buttons_state;
} UserEventMouse;

typedef struct UserEventKeycode {
    int         keycode;
} UserEventKeycode;

typedef struct UserEventGeneric {
    int         type;
    int         code;
    int         value;
} UserEventGeneric;

#endif 

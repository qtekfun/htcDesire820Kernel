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


#ifndef _ANDROID_PROTOCOL_FB_UPDATES_H
#define _ANDROID_PROTOCOL_FB_UPDATES_H

#include "sysemu.h"

#define AFB_REQUEST_REFRESH     1

typedef struct FBUpdateMessage {
    
    uint16_t    x;
    uint16_t    y;
    uint16_t    w;
    uint16_t    h;

    
    uint8_t rect[0];
} FBUpdateMessage;

typedef struct FBRequestHeader {
    
    uint8_t request_type;
} FBRequestHeader;

#endif 

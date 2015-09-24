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

#ifndef _ANDROID_PROTOCOL_CORE_COMMANDS_H
#define _ANDROID_PROTOCOL_CORE_COMMANDS_H


#include "android/hw-sensors.h"
#include "android/protocol/ui-common.h"

#define AUICMD_SET_COARSE_ORIENTATION       1

#define AUICMD_TOGGLE_NETWORK               2

#define AUICMD_TRACE_CONTROL                3

#define AUICMD_CHK_NETWORK_DISABLED         4

#define AUICMD_GET_NETSPEED                 5

#define AUICMD_GET_NETDELAY                 6

#define AUICMD_GET_QEMU_PATH                7

#define AUICMD_GET_LCD_DENSITY              8

typedef struct UICmdSetCoarseOrientation {
    AndroidCoarseOrientation    orient;
} UICmdSetCoarseOrientation;

typedef struct UICmdTraceControl {
    int start;
} UICmdTraceControl;

typedef struct UICmdGetNetSpeed {
    int index;
} UICmdGetNetSpeed;

typedef struct UICmdGetNetSpeedResp {
    int     upload;
    int     download;
    char    name[0];
} UICmdGetNetSpeedResp;

typedef struct UICmdGetNetDelay {
    int index;
} UICmdGetNetDelay;

typedef struct UICmdGetNetDelayResp {
    int     min_ms;
    int     max_ms;
    char    name[0];
} UICmdGetNetDelayResp;

typedef struct UICmdGetQemuPath {
    int     type;
    char    filename[0];
} UICmdGetQemuPath;

typedef struct UICmdGetQemuPathResp {
    
    char    path[0];
} UICmdGetQemuPathResp;

#endif 

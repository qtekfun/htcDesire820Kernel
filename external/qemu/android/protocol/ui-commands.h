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

#ifndef _ANDROID_PROTOCOL_UI_COMMANDS_H
#define _ANDROID_PROTOCOL_UI_COMMANDS_H


#include "android/protocol/ui-common.h"

#define AUICMD_SET_WINDOWS_SCALE        1

#define AUICMD_CHANGE_DISP_BRIGHTNESS   2

typedef struct UICmdSetWindowsScale {
    double  scale;
    int     is_dpi;
} UICmdSetWindowsScale;

typedef struct UICmdChangeDispBrightness {
    int     brightness;
    char    light[0];
} UICmdChangeDispBrightness;

#endif 

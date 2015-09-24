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

#ifndef _ANDROID_PROTOCOL_UI_COMMANDS_API_H
#define _ANDROID_PROTOCOL_UI_COMMANDS_API_H


extern int uicmd_set_window_scale(double scale, int is_dpi);

typedef void  (*AndroidHwLightBrightnessCallback)(void* opaque,
                                                  const char* light,
                                                  int  brightness);

extern int uicmd_set_brightness_change_callback(AndroidHwLightBrightnessCallback callback,
                                                void* opaque);

#endif 

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

#ifndef _ANDROID_PROTOCOL_CORE_COMMANDS_API_H
#define _ANDROID_PROTOCOL_CORE_COMMANDS_API_H


#include "android/android.h"
#include "android/hw-sensors.h"

extern int corecmd_set_coarse_orientation(AndroidCoarseOrientation orient);

extern int corecmd_toggle_network();

extern int corecmd_trace_control(int start);

extern int corecmd_is_network_disabled();

extern int corecmd_get_netspeed(int index, NetworkSpeed** netspeed);

extern int corecmd_get_netdelay(int index, NetworkLatency** netdelay);

extern int corecmd_get_qemu_path(int type,
                                 const char* filename,
                                 char* path,
                                 size_t path_buf_size);

extern int corecmd_get_hw_lcd_density(void);

#endif 

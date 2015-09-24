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

#ifndef _ANDROID_PROTOCOL_UI_COMMON_H
#define _ANDROID_PROTOCOL_UI_COMMON_H


typedef struct UICmdHeader {
    
    uint8_t     cmd_type;

    uint32_t    cmd_param_size;
} UICmdHeader;

typedef struct UICmdRespHeader {
    
    int         result;

    uint32_t    resp_data_size;
} UICmdRespHeader;

#endif 

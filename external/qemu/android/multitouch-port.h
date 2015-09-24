/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_ANDROID_MULTITOUCH_PORT_H_
#define ANDROID_ANDROID_MULTITOUCH_PORT_H_

#include "android/sdk-controller-socket.h"



#define MTFB_JPEG       1
#define MTFB_RGB565     2
#define MTFB_RGB888     3

typedef struct MTFrameHeader {
    
    int         header_size;
    
    int         disp_width;
    
    int         disp_height;
    
    int         x;
    int         y;
    int         w;
    int         h;
    
    int         bpl;
    
    int         bpp;
    
    int         format;
} MTFrameHeader;

typedef struct AndroidMTSPort AndroidMTSPort;

extern AndroidMTSPort* mts_port_create(void* opaque);

extern void mts_port_destroy(AndroidMTSPort* amtp);

extern int mts_port_send_frame(AndroidMTSPort* mtsp,
                               MTFrameHeader* fmt,
                               const uint8_t* fb,
                               on_sdkctl_direct_cb cb,
                               void* cb_opaque,
                               int ydir);

#endif  

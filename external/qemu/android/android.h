/* Copyright (C) 2007 The Android Open Source Project
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
#ifndef  _qemu_android_h
#define  _qemu_android_h

#define  CONFIG_SHAPER  1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


extern double   qemu_net_upload_speed;
extern double   qemu_net_download_speed;

extern int      qemu_net_min_latency;
extern int      qemu_net_max_latency;

extern int      qemu_net_disable;

typedef struct {
    const char*  name;
    const char*  display;
    int          upload;
    int          download;
} NetworkSpeed;

extern const NetworkSpeed   android_netspeeds[];
extern const size_t android_netspeeds_count;

typedef struct {
    const char*  name;
    const char*  display;
    int          min_ms;
    int          max_ms;
} NetworkLatency;

extern const NetworkLatency  android_netdelays[];
extern const size_t android_netdelays_count;

#define  DEFAULT_NETSPEED  "full"
#define  DEFAULT_NETDELAY  "none"

extern void  qemu_polling_enable(void);
extern void  qemu_polling_disable(void);


extern int  android_framebuffer_w;
extern int  android_framebuffer_h;
extern int  android_framebuffer_phys_w;
extern int  android_framebuffer_phys_h;

typedef enum {
    ANDROID_ROTATION_0 = 0,
    ANDROID_ROTATION_90,
    ANDROID_ROTATION_180,
    ANDROID_ROTATION_270
} AndroidRotation;

extern AndroidRotation  android_framebuffer_rotation;


extern int   android_base_port;

extern int   android_parse_network_speed(const char*  speed);

extern int   android_parse_network_latency(const char*  delay);


#define ANDROID_GLSTRING_BUF_SIZE 128
extern char android_gl_vendor[ANDROID_GLSTRING_BUF_SIZE];
extern char android_gl_renderer[ANDROID_GLSTRING_BUF_SIZE];
extern char android_gl_version[ANDROID_GLSTRING_BUF_SIZE];

extern void  android_emulation_setup( void );
extern void  android_emulation_teardown( void );

#endif 

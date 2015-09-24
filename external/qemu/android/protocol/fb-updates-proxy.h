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


#ifndef _ANDROID_PROTOCOL_FB_UPDATES_PROXY_H
#define _ANDROID_PROTOCOL_FB_UPDATES_PROXY_H

typedef struct ProxyFramebuffer ProxyFramebuffer;

ProxyFramebuffer* proxyFb_create(int sock, const char* protocol);

void proxyFb_destroy(ProxyFramebuffer* core_fb);

int proxyFb_get_bits_per_pixel(ProxyFramebuffer* core_fb);

#endif 

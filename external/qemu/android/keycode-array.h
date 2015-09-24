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
#ifndef QEMU_ANDROID_KEYCODE_ARRAY_H
#define QEMU_ANDROID_KEYCODE_ARRAY_H


#define  MAX_KEYCODES   256*2

typedef struct AKeycodeBuffer {
    
    int                 keycode_count;

    
    int                 keycodes[ MAX_KEYCODES ];
} AKeycodeBuffer;

void
android_keycodes_add_key_event( AKeycodeBuffer* keycodes,
                                unsigned       code,
                                unsigned       down );

void
android_keycodes_flush(AKeycodeBuffer* keycodes);

#endif 

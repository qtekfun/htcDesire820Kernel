/* Copyright (C) 2007-2008 The Android Open Source Project
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
#ifndef _android_charmap_h
#define _android_charmap_h

#include "android/keycode.h"
#include "android/keycode-array.h"

typedef struct AKeyEntry {
    unsigned short  code;
    unsigned short  base;
    unsigned short  caps;
    unsigned short  fn;
    unsigned short  caps_fn;
    unsigned short  number;
} AKeyEntry;

#define AKEYCHARMAP_NAME_SIZE   32

typedef struct AKeyCharmap {
    const AKeyEntry*  entries;
    int               num_entries;
    char              name[ AKEYCHARMAP_NAME_SIZE ];
} AKeyCharmap;

void kcm_extract_charmap_name(const char* kcm_file_path,
                              char* charmap_name,
                              int max_len);

const AKeyCharmap* android_get_default_charmap(void);

int android_charmap_setup(const char* kcm_file_path);

void android_charmap_done(void);

const AKeyCharmap* android_get_charmap_by_name(const char* name);

int
android_charmap_reverse_map_unicode(const AKeyCharmap* cmap,
                                    unsigned int unicode,
                                    int  down,
                                    AKeycodeBuffer* keycodes);

const AKeyCharmap* android_get_charmap(void);

const char* android_get_charmap_name(void);

#endif 

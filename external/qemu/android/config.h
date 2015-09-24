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
#ifndef ANDROID_CONFIG_H
#define ANDROID_CONFIG_H

typedef struct AConfig  AConfig;

struct AConfig
{
    AConfig*     next;
    AConfig*     first_child;
    AConfig*     last_child;
    const char*  name;
    const char*  value;
};

extern void   aconfig_load(AConfig*  root, char*  data);

extern int    aconfig_load_file(AConfig*  root, const char*  path);

extern int    aconfig_save_file(AConfig*  root, const char* path);

extern AConfig*  aconfig_node(const char *name, const char *value);

extern AConfig*  aconfig_find(AConfig *root, const char *name);

extern void      aconfig_set(AConfig *root, const char *name, const char *value);


extern int          aconfig_bool    (AConfig *root, const char *name, int _default);
extern unsigned     aconfig_unsigned(AConfig *root, const char *name, unsigned _default);
extern int          aconfig_int     (AConfig *root, const char *name, int _default);
extern const char*  aconfig_str     (AConfig *root, const char *name, const char *_default);

#endif 

/* Copyright (C) 2012 The Android Open Source Project
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

#ifndef _ANDROID_SNAPHOST_ANDROID_H_
#define _ANDROID_SNAPHOST_ANDROID_H_

extern int snaphost_match_configs(IniFile* hw_ini, const char* name);

extern void snaphost_save_config(const char* name);

#endif  


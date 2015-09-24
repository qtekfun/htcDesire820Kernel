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


#ifndef QEMU_ANDROID_CORE_INIT_UI_UTILS_H_
#define QEMU_ANDROID_CORE_INIT_UI_UTILS_H_

void android_core_init_completed(void);

void android_core_init_failure(const char* fmt, ...);

void android_core_init_exit(int exit_status);

#endif  

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

#ifndef _ANDROID_UTILS_FILELOCK_H
#define _ANDROID_UTILS_FILELOCK_H


typedef struct FileLock  FileLock;

extern FileLock*  filelock_create ( const char*  path );
extern void       filelock_release( FileLock*  lock );

#endif 

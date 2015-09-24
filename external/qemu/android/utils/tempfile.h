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

#ifndef _ANDROID_UTILS_TEMPFILE_H
#define _ANDROID_UTILS_TEMPFILE_H


typedef struct TempFile   TempFile;

extern  TempFile*    tempfile_create( void );
extern  const char*  tempfile_path( TempFile*  temp );
extern  void         tempfile_close( TempFile*  temp );

extern void          atexit_close_fd(int  fd);
extern void          atexit_close_fd_remove(int  fd);

#endif 

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
#ifndef _CHARPIPE_H
#define _CHARPIPE_H

#include "qemu-common.h"

extern int  qemu_chr_open_charpipe( CharDriverState* *pfirst, CharDriverState* *psecond );

extern CharDriverState*  qemu_chr_open_buffer( CharDriverState*  endpoint );

extern void charpipe_poll( void );

#endif 

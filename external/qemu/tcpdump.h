/* Copyright (C) 2008 The Android Open Source Project
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
#ifndef _QEMU_TCPDUMP_H
#define _QEMU_TCPDUMP_H

#include <stdint.h>

extern int  qemu_tcpdump_active;

extern int  qemu_tcpdump_start( const char*  filepath );

extern void qemu_tcpdump_stop( void );

extern void qemu_tcpdump_packet( const void*  base, int  len );

extern void  qemu_tcpdump_stats( uint64_t  *pcount, uint64_t*  psize );

#endif 

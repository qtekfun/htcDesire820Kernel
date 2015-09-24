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
#ifndef _modem_driver_h
#define _modem_driver_h

#include "android_modem.h"
#include "qemu-common.h"

extern CharDriverState*  android_modem_cs;

extern AModem  android_modem;

extern void   android_modem_init( int  base_port );

#endif 

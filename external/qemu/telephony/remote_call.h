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
#ifndef _REMOTE_CALL_H
#define _REMOTE_CALL_H

#include "sms.h"

extern int         remote_number_from_port( int  port );

extern int         remote_number_to_port( int  number );

extern int         remote_number_string_to_port( const char*  number );

typedef void   (*RemoteResultFunc)( void*  opaque, int  success );

typedef enum {
    REMOTE_CALL_DIAL = 0,
    REMOTE_CALL_BUSY,
    REMOTE_CALL_HANGUP,
    REMOTE_CALL_HOLD,
    REMOTE_CALL_ACCEPT,
    REMOTE_CALL_SMS
} RemoteCallType;

extern  int     remote_call_dial( const char*       to_number,
                                  int               from_port,
                                  RemoteResultFunc  result_func,
                                  void*             result_opaque );

extern int      remote_call_sms( const char*   number, int  from_port, SmsPDU  pdu );

extern void     remote_call_other( const char*  to_number, int  from_port, RemoteCallType  type );

extern void     remote_call_cancel( const char*  to_number, int from_port );

#endif 

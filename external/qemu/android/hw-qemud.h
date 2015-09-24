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
#ifndef _android_qemud_h
#define _android_qemud_h

#include "qemu-common.h"



extern void  android_qemud_init( void );

extern CharDriverState*  android_qemud_get_cs( void );

extern int  android_qemud_get_channel( const char*  name, CharDriverState* *pcs );

extern int  android_qemud_set_channel( const char*  name, CharDriverState*  peer_cs );

#define  ANDROID_QEMUD_GSM      "gsm"
#define  ANDROID_QEMUD_GPS      "gps"
#define  ANDROID_QEMUD_CONTROL  "control"
#define  ANDROID_QEMUD_SENSORS  "sensors"


typedef struct QemudClient   QemudClient;
typedef struct QemudService  QemudService;


typedef void (*QemudClientClose)( void*  opaque );

typedef void (*QemudClientRecv) ( void*  opaque, uint8_t*  msg, int  msglen, QemudClient*  client );

typedef void (*QemudClientSave) ( QEMUFile*  f, QemudClient*  client, void*  opaque );

typedef int (*QemudClientLoad) ( QEMUFile*  f, QemudClient*  client, void*  opaque );

extern QemudClient*  qemud_client_new( QemudService*      service,
                                        int               channel_id,
                                        const char*       client_param,
                                        void*             clie_opaque,
                                        QemudClientRecv   clie_recv,
                                        QemudClientClose  clie_close,
                                        QemudClientSave   clie_save,
                                        QemudClientLoad   clie_load );

extern void           qemud_client_set_framing( QemudClient*  client, int  enabled );

extern void   qemud_client_send ( QemudClient*  client, const uint8_t*  msg, int  msglen );

extern void   qemud_client_close( QemudClient*  client );


typedef QemudClient*  (*QemudServiceConnect)( void*   opaque,
                                              QemudService*  service,
                                              int  channel,
                                              const char* client_param );

typedef void (*QemudServiceSave) ( QEMUFile*  f, QemudService*  service, void*  opaque );

typedef int (*QemudServiceLoad) ( QEMUFile*  f, QemudService*  service, void*  opaque );

extern QemudService*  qemud_service_register( const char*          serviceName,
                                              int                  max_clients,
                                              void*                serv_opaque,
                                              QemudServiceConnect  serv_connect,
                                              QemudServiceSave     serv_save,
                                              QemudServiceLoad     serv_load);

extern void           qemud_service_broadcast( QemudService*   sv,
                                               const uint8_t*  msg,
                                               int             msglen );

#endif 

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
#ifndef _PROXY_INT_H
#define _PROXY_INT_H

#include "proxy_common.h"
#include "sockets.h"
#include "android/utils/stralloc.h"

extern int  proxy_log;

extern void
proxy_LOG(const char*  fmt, ...);

#define  PROXY_LOG(...)   \
    do { if (proxy_log) proxy_LOG(__VA_ARGS__); } while (0)



enum {
    PROXY_SELECT_READ  = (1 << 0),
    PROXY_SELECT_WRITE = (1 << 1),
    PROXY_SELECT_ERROR = (1 << 2)
};

typedef struct {
    int*     pcount;
    fd_set*  reads;
    fd_set*  writes;
    fd_set*  errors;
} ProxySelect;

extern void     proxy_select_set( ProxySelect*  sel,
                                  int           fd,
                                  unsigned      flags );

extern unsigned  proxy_select_poll( ProxySelect*  sel, int  fd );



typedef struct ProxyConnection   ProxyConnection;
typedef struct ProxyService      ProxyService;

typedef void              (*ProxyConnectionFreeFunc)   ( ProxyConnection*  conn );

typedef void              (*ProxyConnectionSelectFunc) ( ProxyConnection*  conn,
                                                         ProxySelect*      sel );

typedef void              (*ProxyConnectionPollFunc)   ( ProxyConnection*  conn,
                                                         ProxySelect*      sel );


struct ProxyConnection {
    int                 socket;
    SockAddress         address;  
    ProxyConnection*    next;
    ProxyConnection*    prev;
    ProxyEventFunc      ev_func;
    void*               ev_opaque;
    ProxyService*       service;

    
    char                name[64];    

    stralloc_t          str[1];      
    int                 str_pos;     
    int                 str_sent;    
    int                 str_recv;    

    
    ProxyConnectionFreeFunc    conn_free;
    ProxyConnectionSelectFunc  conn_select;
    ProxyConnectionPollFunc    conn_poll;

    
};



extern void
proxy_connection_init( ProxyConnection*           conn,
                       int                        socket,
                       SockAddress*               address,
                       ProxyService*              service,
                       ProxyConnectionFreeFunc    conn_free,
                       ProxyConnectionSelectFunc  conn_select,
                       ProxyConnectionPollFunc    conn_poll );

extern void
proxy_connection_done( ProxyConnection*  conn );

extern void
proxy_connection_free( ProxyConnection*  conn,
                       int               keep_alive,
                       ProxyEvent        event );

typedef enum {
    DATA_ERROR     = -1,
    DATA_NEED_MORE =  0,
    DATA_COMPLETED =  1
} DataStatus;

/* try to send data from the connection's buffer to a socket.
 * starting from offset conn->str_pos in the buffer
 *
 * returns DATA_COMPLETED if everything could be written
 * returns DATA_ERROR for a socket disconnection or error
 * returns DATA_NEED_MORE if all data could not be sent.
 *
 * on exit, conn->str_sent contains the number of bytes
 * that were really sent. conn->str_pos will be incremented
 * by conn->str_sent as well.
 *
 * note that in case of success (DATA_COMPLETED), this also
 * performs a proxy_connection_rewind which sets conn->str_pos
 * to 0.
 */
extern DataStatus
proxy_connection_send( ProxyConnection*  conn, int  fd );

extern DataStatus
proxy_connection_receive( ProxyConnection*  conn, int  fd, int  wanted );

extern DataStatus
proxy_connection_receive_line( ProxyConnection*  conn, int  fd );

extern void
proxy_connection_rewind( ProxyConnection*  conn );

extern int
proxy_base64_encode( const char*  src, int  srclen,
                     char*        dst, int  dstlen );

extern int
proxy_resolve_server( SockAddress*   addr,
                      const char*    servername,
                      int            servernamelen,
                      int            serverport );


typedef void              (*ProxyServiceFreeFunc)      ( void*  opaque );

typedef ProxyConnection*  (*ProxyServiceConnectFunc)( void*               opaque,
                                                      SocketType          socket_type,
                                                      const SockAddress*  address );

struct ProxyService {
    void*                      opaque;
    ProxyServiceFreeFunc       serv_free;
    ProxyServiceConnectFunc    serv_connect;
};

extern int
proxy_manager_add_service( ProxyService*  service );


#endif 

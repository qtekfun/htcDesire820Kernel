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
#ifndef _PROXY_HTTP_INT_H
#define _PROXY_HTTP_INT_H

#include "proxy_http.h"
#include "proxy_int.h"

typedef struct HttpService {
    ProxyService        root[1];
    SockAddress         server_addr;  
    char*               footer;      
    int                 footer_len;  
    char                footer0[512];
} HttpService;

extern ProxyConnection*  http_connector_connect(
                                HttpService*   service,
                                SockAddress*   address );

extern ProxyConnection*  http_rewriter_connect(
                                HttpService*   service,
                                SockAddress*   address );


#endif 

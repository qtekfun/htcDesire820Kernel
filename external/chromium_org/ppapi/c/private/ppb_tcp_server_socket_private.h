/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPB_TCP_SERVER_SOCKET_PRIVATE_H_
#define PPAPI_C_PRIVATE_PPB_TCP_SERVER_SOCKET_PRIVATE_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/private/ppb_net_address_private.h"

#define PPB_TCPSERVERSOCKET_PRIVATE_INTERFACE_0_1 \
    "PPB_TCPServerSocket_Private;0.1"
#define PPB_TCPSERVERSOCKET_PRIVATE_INTERFACE_0_2 \
    "PPB_TCPServerSocket_Private;0.2"
#define PPB_TCPSERVERSOCKET_PRIVATE_INTERFACE \
    PPB_TCPSERVERSOCKET_PRIVATE_INTERFACE_0_2



struct PPB_TCPServerSocket_Private_0_2 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsTCPServerSocket)(PP_Resource resource);
  int32_t (*Listen)(PP_Resource tcp_server_socket,
                    const struct PP_NetAddress_Private* addr,
                    int32_t backlog,
                    struct PP_CompletionCallback callback);
  int32_t (*Accept)(PP_Resource tcp_server_socket,
                    PP_Resource* tcp_socket,
                    struct PP_CompletionCallback callback);
  int32_t (*GetLocalAddress)(PP_Resource tcp_server_socket,
                             struct PP_NetAddress_Private* addr);
  void (*StopListening)(PP_Resource tcp_server_socket);
};

typedef struct PPB_TCPServerSocket_Private_0_2 PPB_TCPServerSocket_Private;

struct PPB_TCPServerSocket_Private_0_1 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsTCPServerSocket)(PP_Resource resource);
  int32_t (*Listen)(PP_Resource tcp_server_socket,
                    const struct PP_NetAddress_Private* addr,
                    int32_t backlog,
                    struct PP_CompletionCallback callback);
  int32_t (*Accept)(PP_Resource tcp_server_socket,
                    PP_Resource* tcp_socket,
                    struct PP_CompletionCallback callback);
  void (*StopListening)(PP_Resource tcp_server_socket);
};

#endif  


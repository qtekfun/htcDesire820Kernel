/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPB_TCP_SOCKET_PRIVATE_H_
#define PPAPI_C_PRIVATE_PPB_TCP_SOCKET_PRIVATE_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/c/private/ppb_net_address_private.h"

#define PPB_TCPSOCKET_PRIVATE_INTERFACE_0_3 "PPB_TCPSocket_Private;0.3"
#define PPB_TCPSOCKET_PRIVATE_INTERFACE_0_4 "PPB_TCPSocket_Private;0.4"
#define PPB_TCPSOCKET_PRIVATE_INTERFACE_0_5 "PPB_TCPSocket_Private;0.5"
#define PPB_TCPSOCKET_PRIVATE_INTERFACE PPB_TCPSOCKET_PRIVATE_INTERFACE_0_5



typedef enum {
  
  PP_TCPSOCKETOPTION_PRIVATE_INVALID = 0,
  PP_TCPSOCKETOPTION_PRIVATE_NO_DELAY = 1
} PP_TCPSocketOption_Private;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_TCPSocketOption_Private, 4);

struct PPB_TCPSocket_Private_0_5 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsTCPSocket)(PP_Resource resource);
  int32_t (*Connect)(PP_Resource tcp_socket,
                     const char* host,
                     uint16_t port,
                     struct PP_CompletionCallback callback);
  int32_t (*ConnectWithNetAddress)(PP_Resource tcp_socket,
                                   const struct PP_NetAddress_Private* addr,
                                   struct PP_CompletionCallback callback);
  PP_Bool (*GetLocalAddress)(PP_Resource tcp_socket,
                             struct PP_NetAddress_Private* local_addr);
  PP_Bool (*GetRemoteAddress)(PP_Resource tcp_socket,
                              struct PP_NetAddress_Private* remote_addr);
  int32_t (*SSLHandshake)(PP_Resource tcp_socket,
                          const char* server_name,
                          uint16_t server_port,
                          struct PP_CompletionCallback callback);
  PP_Resource (*GetServerCertificate)(PP_Resource tcp_socket);
  PP_Bool (*AddChainBuildingCertificate)(PP_Resource tcp_socket,
                                         PP_Resource certificate,
                                         PP_Bool is_trusted);
  int32_t (*Read)(PP_Resource tcp_socket,
                  char* buffer,
                  int32_t bytes_to_read,
                  struct PP_CompletionCallback callback);
  /**
   * Writes data to the socket. May perform a partial write. Returns the number
   * of bytes written or an error code.
   * This method won't write more than 1 megabyte, so if |bytes_to_write|
   * exceeds 1 megabyte, it will always perform a partial write.
   * Multiple outstanding write requests are not supported.
   */
  int32_t (*Write)(PP_Resource tcp_socket,
                   const char* buffer,
                   int32_t bytes_to_write,
                   struct PP_CompletionCallback callback);
  void (*Disconnect)(PP_Resource tcp_socket);
  int32_t (*SetOption)(PP_Resource tcp_socket,
                       PP_TCPSocketOption_Private name,
                       struct PP_Var value,
                       struct PP_CompletionCallback callback);
};

typedef struct PPB_TCPSocket_Private_0_5 PPB_TCPSocket_Private;

struct PPB_TCPSocket_Private_0_3 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsTCPSocket)(PP_Resource resource);
  int32_t (*Connect)(PP_Resource tcp_socket,
                     const char* host,
                     uint16_t port,
                     struct PP_CompletionCallback callback);
  int32_t (*ConnectWithNetAddress)(PP_Resource tcp_socket,
                                   const struct PP_NetAddress_Private* addr,
                                   struct PP_CompletionCallback callback);
  PP_Bool (*GetLocalAddress)(PP_Resource tcp_socket,
                             struct PP_NetAddress_Private* local_addr);
  PP_Bool (*GetRemoteAddress)(PP_Resource tcp_socket,
                              struct PP_NetAddress_Private* remote_addr);
  int32_t (*SSLHandshake)(PP_Resource tcp_socket,
                          const char* server_name,
                          uint16_t server_port,
                          struct PP_CompletionCallback callback);
  int32_t (*Read)(PP_Resource tcp_socket,
                  char* buffer,
                  int32_t bytes_to_read,
                  struct PP_CompletionCallback callback);
  int32_t (*Write)(PP_Resource tcp_socket,
                   const char* buffer,
                   int32_t bytes_to_write,
                   struct PP_CompletionCallback callback);
  void (*Disconnect)(PP_Resource tcp_socket);
};

struct PPB_TCPSocket_Private_0_4 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsTCPSocket)(PP_Resource resource);
  int32_t (*Connect)(PP_Resource tcp_socket,
                     const char* host,
                     uint16_t port,
                     struct PP_CompletionCallback callback);
  int32_t (*ConnectWithNetAddress)(PP_Resource tcp_socket,
                                   const struct PP_NetAddress_Private* addr,
                                   struct PP_CompletionCallback callback);
  PP_Bool (*GetLocalAddress)(PP_Resource tcp_socket,
                             struct PP_NetAddress_Private* local_addr);
  PP_Bool (*GetRemoteAddress)(PP_Resource tcp_socket,
                              struct PP_NetAddress_Private* remote_addr);
  int32_t (*SSLHandshake)(PP_Resource tcp_socket,
                          const char* server_name,
                          uint16_t server_port,
                          struct PP_CompletionCallback callback);
  PP_Resource (*GetServerCertificate)(PP_Resource tcp_socket);
  PP_Bool (*AddChainBuildingCertificate)(PP_Resource tcp_socket,
                                         PP_Resource certificate,
                                         PP_Bool is_trusted);
  int32_t (*Read)(PP_Resource tcp_socket,
                  char* buffer,
                  int32_t bytes_to_read,
                  struct PP_CompletionCallback callback);
  int32_t (*Write)(PP_Resource tcp_socket,
                   const char* buffer,
                   int32_t bytes_to_write,
                   struct PP_CompletionCallback callback);
  void (*Disconnect)(PP_Resource tcp_socket);
};

#endif  


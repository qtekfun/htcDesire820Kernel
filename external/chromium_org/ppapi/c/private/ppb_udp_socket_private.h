/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPB_UDP_SOCKET_PRIVATE_H_
#define PPAPI_C_PRIVATE_PPB_UDP_SOCKET_PRIVATE_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/c/private/ppb_net_address_private.h"

#define PPB_UDPSOCKET_PRIVATE_INTERFACE_0_2 "PPB_UDPSocket_Private;0.2"
#define PPB_UDPSOCKET_PRIVATE_INTERFACE_0_3 "PPB_UDPSocket_Private;0.3"
#define PPB_UDPSOCKET_PRIVATE_INTERFACE_0_4 "PPB_UDPSocket_Private;0.4"
#define PPB_UDPSOCKET_PRIVATE_INTERFACE PPB_UDPSOCKET_PRIVATE_INTERFACE_0_4



typedef enum {
  PP_UDPSOCKETFEATURE_PRIVATE_ADDRESS_REUSE = 0,
  PP_UDPSOCKETFEATURE_PRIVATE_BROADCAST = 1,
  PP_UDPSOCKETFEATURE_PRIVATE_COUNT = 2
} PP_UDPSocketFeature_Private;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_UDPSocketFeature_Private, 4);

struct PPB_UDPSocket_Private_0_4 {
  PP_Resource (*Create)(PP_Instance instance_id);
  PP_Bool (*IsUDPSocket)(PP_Resource resource_id);
  int32_t (*SetSocketFeature)(PP_Resource udp_socket,
                              PP_UDPSocketFeature_Private name,
                              struct PP_Var value);
  
  int32_t (*Bind)(PP_Resource udp_socket,
                  const struct PP_NetAddress_Private* addr,
                  struct PP_CompletionCallback callback);
  PP_Bool (*GetBoundAddress)(PP_Resource udp_socket,
                             struct PP_NetAddress_Private* addr);
  int32_t (*RecvFrom)(PP_Resource udp_socket,
                      char* buffer,
                      int32_t num_bytes,
                      struct PP_CompletionCallback callback);
  PP_Bool (*GetRecvFromAddress)(PP_Resource udp_socket,
                                struct PP_NetAddress_Private* addr);
  int32_t (*SendTo)(PP_Resource udp_socket,
                    const char* buffer,
                    int32_t num_bytes,
                    const struct PP_NetAddress_Private* addr,
                    struct PP_CompletionCallback callback);
  
  void (*Close)(PP_Resource udp_socket);
};

typedef struct PPB_UDPSocket_Private_0_4 PPB_UDPSocket_Private;

struct PPB_UDPSocket_Private_0_2 {
  PP_Resource (*Create)(PP_Instance instance_id);
  PP_Bool (*IsUDPSocket)(PP_Resource resource_id);
  int32_t (*Bind)(PP_Resource udp_socket,
                  const struct PP_NetAddress_Private* addr,
                  struct PP_CompletionCallback callback);
  int32_t (*RecvFrom)(PP_Resource udp_socket,
                      char* buffer,
                      int32_t num_bytes,
                      struct PP_CompletionCallback callback);
  PP_Bool (*GetRecvFromAddress)(PP_Resource udp_socket,
                                struct PP_NetAddress_Private* addr);
  int32_t (*SendTo)(PP_Resource udp_socket,
                    const char* buffer,
                    int32_t num_bytes,
                    const struct PP_NetAddress_Private* addr,
                    struct PP_CompletionCallback callback);
  void (*Close)(PP_Resource udp_socket);
};

struct PPB_UDPSocket_Private_0_3 {
  PP_Resource (*Create)(PP_Instance instance_id);
  PP_Bool (*IsUDPSocket)(PP_Resource resource_id);
  int32_t (*Bind)(PP_Resource udp_socket,
                  const struct PP_NetAddress_Private* addr,
                  struct PP_CompletionCallback callback);
  PP_Bool (*GetBoundAddress)(PP_Resource udp_socket,
                             struct PP_NetAddress_Private* addr);
  int32_t (*RecvFrom)(PP_Resource udp_socket,
                      char* buffer,
                      int32_t num_bytes,
                      struct PP_CompletionCallback callback);
  PP_Bool (*GetRecvFromAddress)(PP_Resource udp_socket,
                                struct PP_NetAddress_Private* addr);
  int32_t (*SendTo)(PP_Resource udp_socket,
                    const char* buffer,
                    int32_t num_bytes,
                    const struct PP_NetAddress_Private* addr,
                    struct PP_CompletionCallback callback);
  void (*Close)(PP_Resource udp_socket);
};

#endif  


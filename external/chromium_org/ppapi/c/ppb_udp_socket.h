/* Copyright 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_UDP_SOCKET_H_
#define PPAPI_C_PPB_UDP_SOCKET_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_UDPSOCKET_INTERFACE_1_0 "PPB_UDPSocket;1.0"
#define PPB_UDPSOCKET_INTERFACE PPB_UDPSOCKET_INTERFACE_1_0



typedef enum {
  PP_UDPSOCKET_OPTION_ADDRESS_REUSE = 0,
  PP_UDPSOCKET_OPTION_BROADCAST = 1,
  PP_UDPSOCKET_OPTION_SEND_BUFFER_SIZE = 2,
  PP_UDPSOCKET_OPTION_RECV_BUFFER_SIZE = 3
} PP_UDPSocket_Option;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_UDPSocket_Option, 4);

struct PPB_UDPSocket_1_0 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsUDPSocket)(PP_Resource resource);
  int32_t (*Bind)(PP_Resource udp_socket,
                  PP_Resource addr,
                  struct PP_CompletionCallback callback);
  PP_Resource (*GetBoundAddress)(PP_Resource udp_socket);
  int32_t (*RecvFrom)(PP_Resource udp_socket,
                      char* buffer,
                      int32_t num_bytes,
                      PP_Resource* addr,
                      struct PP_CompletionCallback callback);
  int32_t (*SendTo)(PP_Resource udp_socket,
                    const char* buffer,
                    int32_t num_bytes,
                    PP_Resource addr,
                    struct PP_CompletionCallback callback);
  void (*Close)(PP_Resource udp_socket);
  int32_t (*SetOption)(PP_Resource udp_socket,
                       PP_UDPSocket_Option name,
                       struct PP_Var value,
                       struct PP_CompletionCallback callback);
};

typedef struct PPB_UDPSocket_1_0 PPB_UDPSocket;

#endif  


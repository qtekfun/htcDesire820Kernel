/* Copyright 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_TCP_SOCKET_H_
#define PPAPI_C_PPB_TCP_SOCKET_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_TCPSOCKET_INTERFACE_1_0 "PPB_TCPSocket;1.0"
#define PPB_TCPSOCKET_INTERFACE_1_1 "PPB_TCPSocket;1.1"
#define PPB_TCPSOCKET_INTERFACE PPB_TCPSOCKET_INTERFACE_1_1



typedef enum {
  PP_TCPSOCKET_OPTION_NO_DELAY = 0,
  PP_TCPSOCKET_OPTION_SEND_BUFFER_SIZE = 1,
  PP_TCPSOCKET_OPTION_RECV_BUFFER_SIZE = 2
} PP_TCPSocket_Option;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_TCPSocket_Option, 4);

struct PPB_TCPSocket_1_1 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsTCPSocket)(PP_Resource resource);
  int32_t (*Bind)(PP_Resource tcp_socket,
                  PP_Resource addr,
                  struct PP_CompletionCallback callback);
  int32_t (*Connect)(PP_Resource tcp_socket,
                     PP_Resource addr,
                     struct PP_CompletionCallback callback);
  PP_Resource (*GetLocalAddress)(PP_Resource tcp_socket);
  PP_Resource (*GetRemoteAddress)(PP_Resource tcp_socket);
  int32_t (*Read)(PP_Resource tcp_socket,
                  char* buffer,
                  int32_t bytes_to_read,
                  struct PP_CompletionCallback callback);
  /**
   * Writes data to the socket. The socket must be connected. It may perform a
   * partial write.
   *
   * @param[in] tcp_socket A <code>PP_Resource</code> corresponding to a TCP
   * socket.
   * @param[in] buffer The buffer containing the data to write.
   * @param[in] bytes_to_write The number of bytes to write.
   * @param[in] callback A <code>PP_CompletionCallback</code> to be called upon
   * completion.
   *
   * @return A non-negative number on success to indicate how many bytes have
   * been written; otherwise, an error code from <code>pp_errors.h</code>.
   */
  int32_t (*Write)(PP_Resource tcp_socket,
                   const char* buffer,
                   int32_t bytes_to_write,
                   struct PP_CompletionCallback callback);
  int32_t (*Listen)(PP_Resource tcp_socket,
                    int32_t backlog,
                    struct PP_CompletionCallback callback);
  int32_t (*Accept)(PP_Resource tcp_socket,
                    PP_Resource* accepted_tcp_socket,
                    struct PP_CompletionCallback callback);
  void (*Close)(PP_Resource tcp_socket);
  int32_t (*SetOption)(PP_Resource tcp_socket,
                       PP_TCPSocket_Option name,
                       struct PP_Var value,
                       struct PP_CompletionCallback callback);
};

typedef struct PPB_TCPSocket_1_1 PPB_TCPSocket;

struct PPB_TCPSocket_1_0 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsTCPSocket)(PP_Resource resource);
  int32_t (*Connect)(PP_Resource tcp_socket,
                     PP_Resource addr,
                     struct PP_CompletionCallback callback);
  PP_Resource (*GetLocalAddress)(PP_Resource tcp_socket);
  PP_Resource (*GetRemoteAddress)(PP_Resource tcp_socket);
  int32_t (*Read)(PP_Resource tcp_socket,
                  char* buffer,
                  int32_t bytes_to_read,
                  struct PP_CompletionCallback callback);
  int32_t (*Write)(PP_Resource tcp_socket,
                   const char* buffer,
                   int32_t bytes_to_write,
                   struct PP_CompletionCallback callback);
  void (*Close)(PP_Resource tcp_socket);
  int32_t (*SetOption)(PP_Resource tcp_socket,
                       PP_TCPSocket_Option name,
                       struct PP_Var value,
                       struct PP_CompletionCallback callback);
};

#endif  


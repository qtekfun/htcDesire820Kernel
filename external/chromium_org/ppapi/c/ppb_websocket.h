/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_WEBSOCKET_H_
#define PPAPI_C_PPB_WEBSOCKET_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_WEBSOCKET_INTERFACE_1_0 "PPB_WebSocket;1.0"
#define PPB_WEBSOCKET_INTERFACE PPB_WEBSOCKET_INTERFACE_1_0



typedef enum {
  PP_WEBSOCKETREADYSTATE_INVALID = -1,
  PP_WEBSOCKETREADYSTATE_CONNECTING = 0,
  PP_WEBSOCKETREADYSTATE_OPEN = 1,
  PP_WEBSOCKETREADYSTATE_CLOSING = 2,
  PP_WEBSOCKETREADYSTATE_CLOSED = 3
} PP_WebSocketReadyState;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_WebSocketReadyState, 4);

typedef enum {
  PP_WEBSOCKETSTATUSCODE_NOT_SPECIFIED = 1005,
  PP_WEBSOCKETSTATUSCODE_NORMAL_CLOSURE = 1000,
  PP_WEBSOCKETSTATUSCODE_GOING_AWAY = 1001,
  PP_WEBSOCKETSTATUSCODE_PROTOCOL_ERROR = 1002,
  PP_WEBSOCKETSTATUSCODE_UNSUPPORTED_DATA = 1003,
  PP_WEBSOCKETSTATUSCODE_NO_STATUS_RECEIVED = 1005,
  PP_WEBSOCKETSTATUSCODE_ABNORMAL_CLOSURE = 1006,
  PP_WEBSOCKETSTATUSCODE_INVALID_FRAME_PAYLOAD_DATA = 1007,
  PP_WEBSOCKETSTATUSCODE_POLICY_VIOLATION = 1008,
  PP_WEBSOCKETSTATUSCODE_MESSAGE_TOO_BIG = 1009,
  PP_WEBSOCKETSTATUSCODE_MANDATORY_EXTENSION = 1010,
  PP_WEBSOCKETSTATUSCODE_INTERNAL_SERVER_ERROR = 1011,
  PP_WEBSOCKETSTATUSCODE_TLS_HANDSHAKE = 1015,
  PP_WEBSOCKETSTATUSCODE_USER_REGISTERED_MIN = 3000,
  PP_WEBSOCKETSTATUSCODE_USER_REGISTERED_MAX = 3999,
  PP_WEBSOCKETSTATUSCODE_USER_PRIVATE_MIN = 4000,
  PP_WEBSOCKETSTATUSCODE_USER_PRIVATE_MAX = 4999
} PP_WebSocketCloseCode;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_WebSocketCloseCode, 4);

struct PPB_WebSocket_1_0 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsWebSocket)(PP_Resource resource);
  int32_t (*Connect)(PP_Resource web_socket,
                     struct PP_Var url,
                     const struct PP_Var protocols[],
                     uint32_t protocol_count,
                     struct PP_CompletionCallback callback);
  int32_t (*Close)(PP_Resource web_socket,
                   uint16_t code,
                   struct PP_Var reason,
                   struct PP_CompletionCallback callback);
  int32_t (*ReceiveMessage)(PP_Resource web_socket,
                            struct PP_Var* message,
                            struct PP_CompletionCallback callback);
  int32_t (*SendMessage)(PP_Resource web_socket, struct PP_Var message);
  uint64_t (*GetBufferedAmount)(PP_Resource web_socket);
  uint16_t (*GetCloseCode)(PP_Resource web_socket);
  struct PP_Var (*GetCloseReason)(PP_Resource web_socket);
  PP_Bool (*GetCloseWasClean)(PP_Resource web_socket);
  struct PP_Var (*GetExtensions)(PP_Resource web_socket);
  struct PP_Var (*GetProtocol)(PP_Resource web_socket);
  PP_WebSocketReadyState (*GetReadyState)(PP_Resource web_socket);
  struct PP_Var (*GetURL)(PP_Resource web_socket);
};

typedef struct PPB_WebSocket_1_0 PPB_WebSocket;

#endif  


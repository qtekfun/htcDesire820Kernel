/* Copyright (c) 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_EXTENSIONS_DEV_PPB_EXT_SOCKET_DEV_H_
#define PPAPI_C_EXTENSIONS_DEV_PPB_EXT_SOCKET_DEV_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_EXT_SOCKET_DEV_INTERFACE_0_1 "PPB_Ext_Socket(Dev);0.1"
#define PPB_EXT_SOCKET_DEV_INTERFACE_0_2 "PPB_Ext_Socket(Dev);0.2"
#define PPB_EXT_SOCKET_DEV_INTERFACE PPB_EXT_SOCKET_DEV_INTERFACE_0_2



typedef struct PP_Var PP_Ext_Socket_SocketType_Dev;

typedef struct PP_Var PP_Ext_Socket_CreateOptions_Dev;

typedef struct PP_Var PP_Ext_Socket_CreateInfo_Dev;

typedef struct PP_Var PP_Ext_Socket_AcceptInfo_Dev;

typedef struct PP_Var PP_Ext_Socket_ReadInfo_Dev;

/**
 * A dictionary <code>PP_Var</code> which contains
 * - "bytesWritten" : integer <code>PP_Var</code>
 * The number of bytes sent, or a negative error code.
 */
typedef struct PP_Var PP_Ext_Socket_WriteInfo_Dev;

typedef struct PP_Var PP_Ext_Socket_RecvFromInfo_Dev;

typedef struct PP_Var PP_Ext_Socket_SocketInfo_Dev;

typedef struct PP_Var PP_Ext_Socket_NetworkInterface_Dev;

typedef struct PP_Var PP_Ext_Socket_NetworkInterface_Dev_Array;

struct PPB_Ext_Socket_Dev_0_2 {
  int32_t (*Create)(PP_Instance instance,
                    PP_Ext_Socket_SocketType_Dev type,
                    PP_Ext_Socket_CreateOptions_Dev options,
                    PP_Ext_Socket_CreateInfo_Dev* create_info,
                    struct PP_CompletionCallback callback);
  void (*Destroy)(PP_Instance instance, struct PP_Var socket_id);
  int32_t (*Connect)(PP_Instance instance,
                     struct PP_Var socket_id,
                     struct PP_Var hostname,
                     struct PP_Var port,
                     struct PP_Var* result,
                     struct PP_CompletionCallback callback);
  int32_t (*Bind)(PP_Instance instance,
                  struct PP_Var socket_id,
                  struct PP_Var address,
                  struct PP_Var port,
                  struct PP_Var* result,
                  struct PP_CompletionCallback callback);
  void (*Disconnect)(PP_Instance instance, struct PP_Var socket_id);
  int32_t (*Read)(PP_Instance instance,
                  struct PP_Var socket_id,
                  struct PP_Var buffer_size,
                  PP_Ext_Socket_ReadInfo_Dev* read_info,
                  struct PP_CompletionCallback callback);
  int32_t (*Write)(PP_Instance instance,
                   struct PP_Var socket_id,
                   struct PP_Var data,
                   PP_Ext_Socket_WriteInfo_Dev* write_info,
                   struct PP_CompletionCallback callback);
  int32_t (*RecvFrom)(PP_Instance instance,
                      struct PP_Var socket_id,
                      struct PP_Var buffer_size,
                      PP_Ext_Socket_RecvFromInfo_Dev* recv_from_info,
                      struct PP_CompletionCallback callback);
  int32_t (*SendTo)(PP_Instance instance,
                    struct PP_Var socket_id,
                    struct PP_Var data,
                    struct PP_Var address,
                    struct PP_Var port,
                    PP_Ext_Socket_WriteInfo_Dev* write_info,
                    struct PP_CompletionCallback callback);
  int32_t (*Listen)(PP_Instance instance,
                    struct PP_Var socket_id,
                    struct PP_Var address,
                    struct PP_Var port,
                    struct PP_Var backlog,
                    struct PP_Var* result,
                    struct PP_CompletionCallback callback);
  int32_t (*Accept)(PP_Instance instance,
                    struct PP_Var socket_id,
                    PP_Ext_Socket_AcceptInfo_Dev* accept_info,
                    struct PP_CompletionCallback callback);
  int32_t (*SetKeepAlive)(PP_Instance instance,
                          struct PP_Var socket_id,
                          struct PP_Var enable,
                          struct PP_Var delay,
                          struct PP_Var* result,
                          struct PP_CompletionCallback callback);
  int32_t (*SetNoDelay)(PP_Instance instance,
                        struct PP_Var socket_id,
                        struct PP_Var no_delay,
                        struct PP_Var* result,
                        struct PP_CompletionCallback callback);
  int32_t (*GetInfo)(PP_Instance instance,
                     struct PP_Var socket_id,
                     PP_Ext_Socket_SocketInfo_Dev* result,
                     struct PP_CompletionCallback callback);
  int32_t (*GetNetworkList)(PP_Instance instance,
                            PP_Ext_Socket_NetworkInterface_Dev_Array* result,
                            struct PP_CompletionCallback callback);
  int32_t (*JoinGroup)(PP_Instance instance,
                       struct PP_Var socket_id,
                       struct PP_Var address,
                       struct PP_Var* result,
                       struct PP_CompletionCallback callback);
  int32_t (*LeaveGroup)(PP_Instance instance,
                        struct PP_Var socket_id,
                        struct PP_Var address,
                        struct PP_Var* result,
                        struct PP_CompletionCallback callback);
  int32_t (*SetMulticastTimeToLive)(PP_Instance instance,
                                    struct PP_Var socket_id,
                                    struct PP_Var ttl,
                                    struct PP_Var* result,
                                    struct PP_CompletionCallback callback);
  int32_t (*SetMulticastLoopbackMode)(PP_Instance instance,
                                      struct PP_Var socket_id,
                                      struct PP_Var enabled,
                                      struct PP_Var* result,
                                      struct PP_CompletionCallback callback);
  int32_t (*GetJoinedGroups)(PP_Instance instance,
                             struct PP_Var socket_id,
                             struct PP_Var* groups,
                             struct PP_CompletionCallback callback);
};

typedef struct PPB_Ext_Socket_Dev_0_2 PPB_Ext_Socket_Dev;

struct PPB_Ext_Socket_Dev_0_1 {
  int32_t (*Create)(PP_Instance instance,
                    PP_Ext_Socket_SocketType_Dev type,
                    PP_Ext_Socket_CreateOptions_Dev options,
                    PP_Ext_Socket_CreateInfo_Dev* create_info,
                    struct PP_CompletionCallback callback);
  void (*Destroy)(PP_Instance instance, struct PP_Var socket_id);
  int32_t (*Connect)(PP_Instance instance,
                     struct PP_Var socket_id,
                     struct PP_Var hostname,
                     struct PP_Var port,
                     struct PP_Var* result,
                     struct PP_CompletionCallback callback);
  int32_t (*Bind)(PP_Instance instance,
                  struct PP_Var socket_id,
                  struct PP_Var address,
                  struct PP_Var port,
                  struct PP_Var* result,
                  struct PP_CompletionCallback callback);
  void (*Disconnect)(PP_Instance instance, struct PP_Var socket_id);
  int32_t (*Read)(PP_Instance instance,
                  struct PP_Var socket_id,
                  struct PP_Var buffer_size,
                  PP_Ext_Socket_ReadInfo_Dev* read_info,
                  struct PP_CompletionCallback callback);
  int32_t (*Write)(PP_Instance instance,
                   struct PP_Var socket_id,
                   struct PP_Var data,
                   PP_Ext_Socket_WriteInfo_Dev* write_info,
                   struct PP_CompletionCallback callback);
  int32_t (*RecvFrom)(PP_Instance instance,
                      struct PP_Var socket_id,
                      struct PP_Var buffer_size,
                      PP_Ext_Socket_RecvFromInfo_Dev* recv_from_info,
                      struct PP_CompletionCallback callback);
  int32_t (*SendTo)(PP_Instance instance,
                    struct PP_Var socket_id,
                    struct PP_Var data,
                    struct PP_Var address,
                    struct PP_Var port,
                    PP_Ext_Socket_WriteInfo_Dev* write_info,
                    struct PP_CompletionCallback callback);
  int32_t (*Listen)(PP_Instance instance,
                    struct PP_Var socket_id,
                    struct PP_Var address,
                    struct PP_Var port,
                    struct PP_Var backlog,
                    struct PP_Var* result,
                    struct PP_CompletionCallback callback);
  int32_t (*Accept)(PP_Instance instance,
                    struct PP_Var socket_id,
                    PP_Ext_Socket_AcceptInfo_Dev* accept_info,
                    struct PP_CompletionCallback callback);
  int32_t (*SetKeepAlive)(PP_Instance instance,
                          struct PP_Var socket_id,
                          struct PP_Var enable,
                          struct PP_Var delay,
                          struct PP_Var* result,
                          struct PP_CompletionCallback callback);
  int32_t (*SetNoDelay)(PP_Instance instance,
                        struct PP_Var socket_id,
                        struct PP_Var no_delay,
                        struct PP_Var* result,
                        struct PP_CompletionCallback callback);
  int32_t (*GetInfo)(PP_Instance instance,
                     struct PP_Var socket_id,
                     PP_Ext_Socket_SocketInfo_Dev* result,
                     struct PP_CompletionCallback callback);
  int32_t (*GetNetworkList)(PP_Instance instance,
                            PP_Ext_Socket_NetworkInterface_Dev_Array* result,
                            struct PP_CompletionCallback callback);
};

#endif  


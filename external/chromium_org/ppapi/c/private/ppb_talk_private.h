/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPB_TALK_PRIVATE_H_
#define PPAPI_C_PRIVATE_PPB_TALK_PRIVATE_H_

#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_TALK_PRIVATE_INTERFACE_1_0 "PPB_Talk_Private;1.0"
#define PPB_TALK_PRIVATE_INTERFACE_2_0 "PPB_Talk_Private;2.0"
#define PPB_TALK_PRIVATE_INTERFACE PPB_TALK_PRIVATE_INTERFACE_2_0



typedef enum {
  PP_TALKPERMISSION_SCREENCAST,
  PP_TALKPERMISSION_REMOTING,
  PP_TALKPERMISSION_REMOTING_CONTINUE,
  PP_TALKPERMISSION_NUM_PERMISSIONS
} PP_TalkPermission;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_TalkPermission, 4);

typedef enum {
  PP_TALKEVENT_TERMINATE,
  PP_TALKEVENT_ERROR,
  PP_TALKEVENT_NUM_EVENTS
} PP_TalkEvent;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_TalkEvent, 4);

typedef void (*PP_TalkEventCallback)(void* user_data, PP_TalkEvent event);

struct PPB_Talk_Private_2_0 {
  PP_Resource (*Create)(PP_Instance instance);
  int32_t (*RequestPermission)(PP_Resource talk_resource,
                               PP_TalkPermission permission,
                               struct PP_CompletionCallback callback);
  int32_t (*StartRemoting)(PP_Resource talk_resource,
                           PP_TalkEventCallback event_callback,
                           void* user_data,
                           struct PP_CompletionCallback callback);
  int32_t (*StopRemoting)(PP_Resource talk_resource,
                          struct PP_CompletionCallback callback);
};

typedef struct PPB_Talk_Private_2_0 PPB_Talk_Private;

struct PPB_Talk_Private_1_0 {
  PP_Resource (*Create)(PP_Instance instance);
  int32_t (*GetPermission)(PP_Resource talk_resource,
                           struct PP_CompletionCallback callback);
};

#endif  


/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_MESSAGE_LOOP_H_
#define PPAPI_C_PPB_MESSAGE_LOOP_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_MESSAGELOOP_INTERFACE_1_0 "PPB_MessageLoop;1.0"
#define PPB_MESSAGELOOP_INTERFACE PPB_MESSAGELOOP_INTERFACE_1_0



struct PPB_MessageLoop_1_0 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Resource (*GetForMainThread)(void);
  PP_Resource (*GetCurrent)(void);
  int32_t (*AttachToCurrentThread)(PP_Resource message_loop);
  int32_t (*Run)(PP_Resource message_loop);
  int32_t (*PostWork)(PP_Resource message_loop,
                      struct PP_CompletionCallback callback,
                      int64_t delay_ms);
  int32_t (*PostQuit)(PP_Resource message_loop, PP_Bool should_destroy);
};

typedef struct PPB_MessageLoop_1_0 PPB_MessageLoop;

#endif  


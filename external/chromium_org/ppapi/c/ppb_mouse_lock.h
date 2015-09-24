/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_MOUSE_LOCK_H_
#define PPAPI_C_PPB_MOUSE_LOCK_H_

#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_MOUSELOCK_INTERFACE_1_0 "PPB_MouseLock;1.0"
#define PPB_MOUSELOCK_INTERFACE PPB_MOUSELOCK_INTERFACE_1_0



struct PPB_MouseLock_1_0 {
  int32_t (*LockMouse)(PP_Instance instance,
                       struct PP_CompletionCallback callback);
  void (*UnlockMouse)(PP_Instance instance);
};

typedef struct PPB_MouseLock_1_0 PPB_MouseLock;

#endif  


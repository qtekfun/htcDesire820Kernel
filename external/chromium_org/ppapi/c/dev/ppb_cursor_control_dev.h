/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_CURSOR_CONTROL_DEV_H_
#define PPAPI_C_DEV_PPB_CURSOR_CONTROL_DEV_H_

#include "ppapi/c/dev/pp_cursor_type_dev.h"
#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_point.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_CURSOR_CONTROL_DEV_INTERFACE_0_4 "PPB_CursorControl(Dev);0.4"
#define PPB_CURSOR_CONTROL_DEV_INTERFACE PPB_CURSOR_CONTROL_DEV_INTERFACE_0_4



struct PPB_CursorControl_Dev_0_4 {
  PP_Bool (*SetCursor)(PP_Instance instance,
                       enum PP_CursorType_Dev type,
                       PP_Resource custom_image,
                       const struct PP_Point* hot_spot);
  PP_Bool (*LockCursor)(PP_Instance instance);
  PP_Bool (*UnlockCursor)(PP_Instance instance);
  PP_Bool (*HasCursorLock)(PP_Instance instance);
  PP_Bool (*CanLockCursor)(PP_Instance instance);
};

typedef struct PPB_CursorControl_Dev_0_4 PPB_CursorControl_Dev;

#endif  


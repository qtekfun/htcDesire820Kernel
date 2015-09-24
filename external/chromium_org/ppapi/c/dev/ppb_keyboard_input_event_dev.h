/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_KEYBOARD_INPUT_EVENT_DEV_H_
#define PPAPI_C_DEV_PPB_KEYBOARD_INPUT_EVENT_DEV_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_KEYBOARD_INPUT_EVENT_DEV_INTERFACE_0_2 \
    "PPB_KeyboardInputEvent(Dev);0.2"
#define PPB_KEYBOARD_INPUT_EVENT_DEV_INTERFACE \
    PPB_KEYBOARD_INPUT_EVENT_DEV_INTERFACE_0_2



struct PPB_KeyboardInputEvent_Dev_0_2 {
  PP_Bool (*SetUsbKeyCode)(PP_Resource key_event, uint32_t usb_key_code);
  uint32_t (*GetUsbKeyCode)(PP_Resource key_event);
  struct PP_Var (*GetCode)(PP_Resource key_event);
};

typedef struct PPB_KeyboardInputEvent_Dev_0_2 PPB_KeyboardInputEvent_Dev;

#endif  


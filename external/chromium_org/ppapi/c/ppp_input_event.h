/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPP_INPUT_EVENT_H_
#define PPAPI_C_PPP_INPUT_EVENT_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPP_INPUT_EVENT_INTERFACE_0_1 "PPP_InputEvent;0.1"
#define PPP_INPUT_EVENT_INTERFACE PPP_INPUT_EVENT_INTERFACE_0_1



struct PPP_InputEvent_0_1 {
  PP_Bool (*HandleInputEvent)(PP_Instance instance, PP_Resource input_event);
};

typedef struct PPP_InputEvent_0_1 PPP_InputEvent;

#endif  


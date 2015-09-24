/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PP_INPUT_EVENT_H_
#define PPAPI_C_PP_INPUT_EVENT_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/ppb_input_event.h"



struct PP_InputEvent_Key {
  
  uint32_t modifier;
  uint32_t key_code;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_InputEvent_Key, 8);

struct PP_InputEvent_Character {
  
  uint32_t modifier;
  char text[5];
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_InputEvent_Character, 12);

struct PP_InputEvent_Mouse {
  uint32_t modifier;
  PP_InputEvent_MouseButton button;
  float x;
  float y;
  
  int32_t click_count;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_InputEvent_Mouse, 20);

struct PP_InputEvent_Wheel {
  uint32_t modifier;
  float delta_x;
  float delta_y;
  float wheel_ticks_x;
  
  float wheel_ticks_y;
  PP_Bool scroll_by_page;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_InputEvent_Wheel, 24);

#endif  


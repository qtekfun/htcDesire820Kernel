/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_GAMEPAD_H_
#define PPAPI_C_PPB_GAMEPAD_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_GAMEPAD_INTERFACE_1_0 "PPB_Gamepad;1.0"
#define PPB_GAMEPAD_INTERFACE PPB_GAMEPAD_INTERFACE_1_0



struct PP_GamepadSampleData {
  uint32_t axes_length;
  float axes[16];
  uint32_t buttons_length;
  float buttons[32];
  double timestamp;
  uint16_t id[128];
  PP_Bool connected;
  
  char unused_pad_[4];
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_GamepadSampleData, 472);

struct PP_GamepadsSampleData {
  uint32_t length;
  
  char unused_pad_[4];
  struct PP_GamepadSampleData items[4];
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_GamepadsSampleData, 1896);

struct PPB_Gamepad_1_0 {
  void (*Sample)(PP_Instance instance, struct PP_GamepadsSampleData* data);
};

typedef struct PPB_Gamepad_1_0 PPB_Gamepad;

#endif  


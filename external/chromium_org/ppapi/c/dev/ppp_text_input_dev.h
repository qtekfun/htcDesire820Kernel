/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPP_TEXT_INPUT_DEV_H_
#define PPAPI_C_DEV_PPP_TEXT_INPUT_DEV_H_

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"

#define PPP_TEXTINPUT_DEV_INTERFACE_0_1 "PPP_TextInput(Dev);0.1"
#define PPP_TEXTINPUT_DEV_INTERFACE PPP_TEXTINPUT_DEV_INTERFACE_0_1



struct PPP_TextInput_Dev_0_1 {
  void (*RequestSurroundingText)(PP_Instance instance,
                                 uint32_t desired_number_of_characters);
};

typedef struct PPP_TextInput_Dev_0_1 PPP_TextInput_Dev;

#endif  


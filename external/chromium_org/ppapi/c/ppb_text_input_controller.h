/* Copyright 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_TEXT_INPUT_CONTROLLER_H_
#define PPAPI_C_PPB_TEXT_INPUT_CONTROLLER_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_point.h"
#include "ppapi/c/pp_rect.h"
#include "ppapi/c/pp_size.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_TEXTINPUTCONTROLLER_INTERFACE_1_0 "PPB_TextInputController;1.0"
#define PPB_TEXTINPUTCONTROLLER_INTERFACE PPB_TEXTINPUTCONTROLLER_INTERFACE_1_0



typedef enum {
  PP_TEXTINPUT_TYPE_NONE = 0,
  PP_TEXTINPUT_TYPE_TEXT = 1,
  PP_TEXTINPUT_TYPE_PASSWORD = 2,
  PP_TEXTINPUT_TYPE_SEARCH = 3,
  PP_TEXTINPUT_TYPE_EMAIL = 4,
  PP_TEXTINPUT_TYPE_NUMBER = 5,
  PP_TEXTINPUT_TYPE_TELEPHONE = 6,
  PP_TEXTINPUT_TYPE_URL = 7
} PP_TextInput_Type;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_TextInput_Type, 4);

struct PPB_TextInputController_1_0 {
  void (*SetTextInputType)(PP_Instance instance, PP_TextInput_Type type);
  void (*UpdateCaretPosition)(PP_Instance instance,
                              const struct PP_Rect* caret);
  void (*CancelCompositionText)(PP_Instance instance);
  void (*UpdateSurroundingText)(PP_Instance instance,
                                struct PP_Var text,
                                uint32_t caret,
                                uint32_t anchor);
};

typedef struct PPB_TextInputController_1_0 PPB_TextInputController;

#endif  


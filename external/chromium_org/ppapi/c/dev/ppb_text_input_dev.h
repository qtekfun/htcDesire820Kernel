/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_TEXT_INPUT_DEV_H_
#define PPAPI_C_DEV_PPB_TEXT_INPUT_DEV_H_

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_point.h"
#include "ppapi/c/pp_rect.h"
#include "ppapi/c/pp_size.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_TEXTINPUT_DEV_INTERFACE_0_1 "PPB_TextInput(Dev);0.1"
#define PPB_TEXTINPUT_DEV_INTERFACE_0_2 "PPB_TextInput(Dev);0.2"
#define PPB_TEXTINPUT_DEV_INTERFACE PPB_TEXTINPUT_DEV_INTERFACE_0_2



typedef enum {
  PP_TEXTINPUT_TYPE_DEV_NONE = 0,
  PP_TEXTINPUT_TYPE_DEV_TEXT = 1,
  PP_TEXTINPUT_TYPE_DEV_PASSWORD = 2,
  PP_TEXTINPUT_TYPE_DEV_SEARCH = 3,
  PP_TEXTINPUT_TYPE_DEV_EMAIL = 4,
  PP_TEXTINPUT_TYPE_DEV_NUMBER = 5,
  PP_TEXTINPUT_TYPE_DEV_TELEPHONE = 6,
  PP_TEXTINPUT_TYPE_DEV_URL = 7
} PP_TextInput_Type_Dev;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_TextInput_Type_Dev, 4);

struct PPB_TextInput_Dev_0_2 {
  void (*SetTextInputType)(PP_Instance instance, PP_TextInput_Type_Dev type);
  void (*UpdateCaretPosition)(PP_Instance instance,
                              const struct PP_Rect* caret,
                              const struct PP_Rect* bounding_box);
  void (*CancelCompositionText)(PP_Instance instance);
  void (*UpdateSurroundingText)(PP_Instance instance,
                                const char* text,
                                uint32_t caret,
                                uint32_t anchor);
  void (*SelectionChanged)(PP_Instance instance);
};

typedef struct PPB_TextInput_Dev_0_2 PPB_TextInput_Dev;

struct PPB_TextInput_Dev_0_1 {
  void (*SetTextInputType)(PP_Instance instance, PP_TextInput_Type_Dev type);
  void (*UpdateCaretPosition)(PP_Instance instance,
                              const struct PP_Rect* caret,
                              const struct PP_Rect* bounding_box);
  void (*CancelCompositionText)(PP_Instance instance);
};

#endif  


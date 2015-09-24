/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef PPAPI_C_DEV_PPB_CHAR_SET_DEV_H_
#define PPAPI_C_DEV_PPB_CHAR_SET_DEV_H_

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_CHAR_SET_DEV_INTERFACE_0_4 "PPB_CharSet(Dev);0.4"
#define PPB_CHAR_SET_DEV_INTERFACE PPB_CHAR_SET_DEV_INTERFACE_0_4

enum PP_CharSet_ConversionError {
  
  
  PP_CHARSET_CONVERSIONERROR_FAIL,

  
  
  PP_CHARSET_CONVERSIONERROR_SKIP,

  
  
  
  
  
  PP_CHARSET_CONVERSIONERROR_SUBSTITUTE
};
PP_COMPILE_ASSERT_ENUM_SIZE_IN_BYTES(PP_CharSet_ConversionError, 4);

struct PPB_CharSet_Dev_0_4 {
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  char* (*UTF16ToCharSet)(PP_Instance instance,
                          const uint16_t* utf16, uint32_t utf16_len,
                          const char* output_char_set,
                          enum PP_CharSet_ConversionError on_error,
                          uint32_t* output_length);

  
  
  
  
  
  
  
  
  uint16_t* (*CharSetToUTF16)(PP_Instance instance,
                              const char* input, uint32_t input_len,
                              const char* input_char_set,
                              enum PP_CharSet_ConversionError on_error,
                              uint32_t* output_length);

  
  
  
  
  
  
  struct PP_Var (*GetDefaultCharSet)(PP_Instance instance);
};

typedef struct PPB_CharSet_Dev_0_4 PPB_CharSet_Dev;

#endif  

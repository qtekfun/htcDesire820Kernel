/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_TRUSTED_PPB_CHAR_SET_TRUSTED_H_
#define PPAPI_C_TRUSTED_PPB_CHAR_SET_TRUSTED_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_CHARSET_TRUSTED_INTERFACE_1_0 "PPB_CharSet_Trusted;1.0"
#define PPB_CHARSET_TRUSTED_INTERFACE PPB_CHARSET_TRUSTED_INTERFACE_1_0



typedef enum {
  PP_CHARSET_TRUSTED_CONVERSIONERROR_FAIL,
  PP_CHARSET_TRUSTED_CONVERSIONERROR_SKIP,
  PP_CHARSET_TRUSTED_CONVERSIONERROR_SUBSTITUTE
} PP_CharSet_Trusted_ConversionError;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_CharSet_Trusted_ConversionError, 4);

struct PPB_CharSet_Trusted_1_0 {
  PP_Bool (*UTF16ToCharSet)(const uint16_t utf16[],
                            uint32_t utf16_len,
                            const char* output_char_set,
                            PP_CharSet_Trusted_ConversionError on_error,
                            char* output_buffer,
                            uint32_t* output_length);
  PP_Bool (*CharSetToUTF16)(const char* input,
                            uint32_t input_len,
                            const char* input_char_set,
                            PP_CharSet_Trusted_ConversionError on_error,
                            uint16_t* output_buffer,
                            uint32_t* output_utf16_length);
  struct PP_Var (*GetDefaultCharSet)(PP_Instance instance);
};

typedef struct PPB_CharSet_Trusted_1_0 PPB_CharSet_Trusted;

#endif  


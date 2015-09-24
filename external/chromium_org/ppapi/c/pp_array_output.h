/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PP_ARRAY_OUTPUT_H_
#define PPAPI_C_PP_ARRAY_OUTPUT_H_

#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"



typedef void* (*PP_ArrayOutput_GetDataBuffer)(void* user_data,
                                              uint32_t element_count,
                                              uint32_t element_size);

struct PP_ArrayOutput {
  PP_ArrayOutput_GetDataBuffer GetDataBuffer;
  void* user_data;
};

#endif  


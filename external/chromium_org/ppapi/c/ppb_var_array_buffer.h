/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_VAR_ARRAY_BUFFER_H_
#define PPAPI_C_PPB_VAR_ARRAY_BUFFER_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_VAR_ARRAY_BUFFER_INTERFACE_1_0 "PPB_VarArrayBuffer;1.0"
#define PPB_VAR_ARRAY_BUFFER_INTERFACE PPB_VAR_ARRAY_BUFFER_INTERFACE_1_0



struct PPB_VarArrayBuffer_1_0 {
  struct PP_Var (*Create)(uint32_t size_in_bytes);
  PP_Bool (*ByteLength)(struct PP_Var array, uint32_t* byte_length);
  void* (*Map)(struct PP_Var array);
  void (*Unmap)(struct PP_Var array);
};

typedef struct PPB_VarArrayBuffer_1_0 PPB_VarArrayBuffer;

#endif  


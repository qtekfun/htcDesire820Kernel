/* Copyright 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_VAR_ARRAY_H_
#define PPAPI_C_PPB_VAR_ARRAY_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_VAR_ARRAY_INTERFACE_1_0 "PPB_VarArray;1.0"
#define PPB_VAR_ARRAY_INTERFACE PPB_VAR_ARRAY_INTERFACE_1_0



struct PPB_VarArray_1_0 {
  struct PP_Var (*Create)(void);
  struct PP_Var (*Get)(struct PP_Var array, uint32_t index);
  PP_Bool (*Set)(struct PP_Var array, uint32_t index, struct PP_Var value);
  uint32_t (*GetLength)(struct PP_Var array);
  PP_Bool (*SetLength)(struct PP_Var array, uint32_t length);
};

typedef struct PPB_VarArray_1_0 PPB_VarArray;

#endif  


/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_VAR_H_
#define PPAPI_C_PPB_VAR_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_VAR_INTERFACE_1_0 "PPB_Var;1.0"
#define PPB_VAR_INTERFACE_1_1 "PPB_Var;1.1"
#define PPB_VAR_INTERFACE PPB_VAR_INTERFACE_1_1



struct PPB_Var_1_1 {
  void (*AddRef)(struct PP_Var var);
  void (*Release)(struct PP_Var var);
  struct PP_Var (*VarFromUtf8)(const char* data, uint32_t len);
  const char* (*VarToUtf8)(struct PP_Var var, uint32_t* len);
};

typedef struct PPB_Var_1_1 PPB_Var;

struct PPB_Var_1_0 {
  void (*AddRef)(struct PP_Var var);
  void (*Release)(struct PP_Var var);
  struct PP_Var (*VarFromUtf8)(PP_Module module,
                               const char* data,
                               uint32_t len);
  const char* (*VarToUtf8)(struct PP_Var var, uint32_t* len);
};

#endif  


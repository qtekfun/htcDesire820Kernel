/* Copyright 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_VAR_DICTIONARY_H_
#define PPAPI_C_PPB_VAR_DICTIONARY_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_VAR_DICTIONARY_INTERFACE_1_0 "PPB_VarDictionary;1.0"
#define PPB_VAR_DICTIONARY_INTERFACE PPB_VAR_DICTIONARY_INTERFACE_1_0



struct PPB_VarDictionary_1_0 {
  struct PP_Var (*Create)(void);
  struct PP_Var (*Get)(struct PP_Var dict, struct PP_Var key);
  PP_Bool (*Set)(struct PP_Var dict, struct PP_Var key, struct PP_Var value);
  void (*Delete)(struct PP_Var dict, struct PP_Var key);
  PP_Bool (*HasKey)(struct PP_Var dict, struct PP_Var key);
  struct PP_Var (*GetKeys)(struct PP_Var dict);
};

typedef struct PPB_VarDictionary_1_0 PPB_VarDictionary;

#endif  


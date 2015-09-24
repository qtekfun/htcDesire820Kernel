/* Copyright 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_VAR_RESOURCE_DEV_H_
#define PPAPI_C_DEV_PPB_VAR_RESOURCE_DEV_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_VAR_RESOURCE_DEV_INTERFACE_0_1 "PPB_VarResource(Dev);0.1"
#define PPB_VAR_RESOURCE_DEV_INTERFACE PPB_VAR_RESOURCE_DEV_INTERFACE_0_1



struct PPB_VarResource_Dev_0_1 {
  PP_Resource (*VarToResource)(struct PP_Var var);
  struct PP_Var (*VarFromResource)(PP_Resource resource);
};

typedef struct PPB_VarResource_Dev_0_1 PPB_VarResource_Dev;

#endif  


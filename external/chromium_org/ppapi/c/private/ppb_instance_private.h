/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPB_INSTANCE_PRIVATE_H_
#define PPAPI_C_PRIVATE_PPB_INSTANCE_PRIVATE_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_INSTANCE_PRIVATE_INTERFACE_0_1 "PPB_Instance_Private;0.1"
#define PPB_INSTANCE_PRIVATE_INTERFACE PPB_INSTANCE_PRIVATE_INTERFACE_0_1



typedef enum {
  
  PP_EXTERNAL_PLUGIN_OK = 0,
  
  PP_EXTERNAL_PLUGIN_FAILED = 1,
  
  PP_EXTERNAL_PLUGIN_ERROR_MODULE = 2,
  
  PP_EXTERNAL_PLUGIN_ERROR_INSTANCE = 3
} PP_ExternalPluginResult;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_ExternalPluginResult, 4);

struct PPB_Instance_Private_0_1 {
  struct PP_Var (*GetWindowObject)(PP_Instance instance);
  struct PP_Var (*GetOwnerElementObject)(PP_Instance instance);
  struct PP_Var (*ExecuteScript)(PP_Instance instance,
                                 struct PP_Var script,
                                 struct PP_Var* exception);
};

typedef struct PPB_Instance_Private_0_1 PPB_Instance_Private;

#endif  


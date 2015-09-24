/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPP_INSTANCE_PRIVATE_H_
#define PPAPI_C_PRIVATE_PPP_INSTANCE_PRIVATE_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPP_INSTANCE_PRIVATE_INTERFACE_0_1 "PPP_Instance_Private;0.1"
#define PPP_INSTANCE_PRIVATE_INTERFACE PPP_INSTANCE_PRIVATE_INTERFACE_0_1



struct PPP_Instance_Private_0_1 {
  struct PP_Var (*GetInstanceObject)(PP_Instance instance);
};

typedef struct PPP_Instance_Private_0_1 PPP_Instance_Private;

#endif  


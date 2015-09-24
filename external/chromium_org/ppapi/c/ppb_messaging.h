/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_MESSAGING_H_
#define PPAPI_C_PPB_MESSAGING_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_MESSAGING_INTERFACE_1_0 "PPB_Messaging;1.0"
#define PPB_MESSAGING_INTERFACE PPB_MESSAGING_INTERFACE_1_0



struct PPB_Messaging_1_0 {
  void (*PostMessage)(PP_Instance instance, struct PP_Var message);
};

typedef struct PPB_Messaging_1_0 PPB_Messaging;

#endif  


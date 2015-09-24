/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPP_MESSAGING_H_
#define PPAPI_C_PPP_MESSAGING_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPP_MESSAGING_INTERFACE_1_0 "PPP_Messaging;1.0"
#define PPP_MESSAGING_INTERFACE PPP_MESSAGING_INTERFACE_1_0



struct PPP_Messaging_1_0 {
  void (*HandleMessage)(PP_Instance instance, struct PP_Var message);
};

typedef struct PPP_Messaging_1_0 PPP_Messaging;

#endif  


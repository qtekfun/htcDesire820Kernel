/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_CONSOLE_H_
#define PPAPI_C_PPB_CONSOLE_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_CONSOLE_INTERFACE_1_0 "PPB_Console;1.0"
#define PPB_CONSOLE_INTERFACE PPB_CONSOLE_INTERFACE_1_0



typedef enum {
  PP_LOGLEVEL_TIP = 0,
  PP_LOGLEVEL_LOG = 1,
  PP_LOGLEVEL_WARNING = 2,
  PP_LOGLEVEL_ERROR = 3
} PP_LogLevel;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_LogLevel, 4);

struct PPB_Console_1_0 {
  void (*Log)(PP_Instance instance, PP_LogLevel level, struct PP_Var value);
  void (*LogWithSource)(PP_Instance instance,
                        PP_LogLevel level,
                        struct PP_Var source,
                        struct PP_Var value);
};

typedef struct PPB_Console_1_0 PPB_Console;

#endif  


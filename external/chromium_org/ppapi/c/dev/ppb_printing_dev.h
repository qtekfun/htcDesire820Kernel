/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_PRINTING_DEV_H_
#define PPAPI_C_DEV_PPB_PRINTING_DEV_H_

#include "ppapi/c/dev/pp_print_settings_dev.h"
#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_point.h"
#include "ppapi/c/pp_rect.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_size.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_PRINTING_DEV_INTERFACE_0_7 "PPB_Printing(Dev);0.7"
#define PPB_PRINTING_DEV_INTERFACE PPB_PRINTING_DEV_INTERFACE_0_7



struct PPB_Printing_Dev_0_7 {
  PP_Resource (*Create)(PP_Instance instance);
  int32_t (*GetDefaultPrintSettings)(
      PP_Resource resource,
      struct PP_PrintSettings_Dev* print_settings,
      struct PP_CompletionCallback callback);
};

typedef struct PPB_Printing_Dev_0_7 PPB_Printing_Dev;

#endif  


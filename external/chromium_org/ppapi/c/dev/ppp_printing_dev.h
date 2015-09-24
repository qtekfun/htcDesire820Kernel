/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPP_PRINTING_DEV_H_
#define PPAPI_C_DEV_PPP_PRINTING_DEV_H_

#include "ppapi/c/dev/pp_print_settings_dev.h"
#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_point.h"
#include "ppapi/c/pp_rect.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_size.h"
#include "ppapi/c/pp_stdint.h"

#define PPP_PRINTING_DEV_INTERFACE_0_6 "PPP_Printing(Dev);0.6"
#define PPP_PRINTING_DEV_INTERFACE PPP_PRINTING_DEV_INTERFACE_0_6



struct PP_PrintPageNumberRange_Dev {
  uint32_t first_page_number;
  uint32_t last_page_number;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_PrintPageNumberRange_Dev, 8);

struct PPP_Printing_Dev_0_6 {
  uint32_t (*QuerySupportedFormats)(PP_Instance instance);
  int32_t (*Begin)(PP_Instance instance,
                   const struct PP_PrintSettings_Dev* print_settings);
  PP_Resource (*PrintPages)(
      PP_Instance instance,
      const struct PP_PrintPageNumberRange_Dev* page_ranges,
      uint32_t page_range_count);
  
  void (*End)(PP_Instance instance);
  PP_Bool (*IsScalingDisabled)(PP_Instance instance);
};

typedef struct PPP_Printing_Dev_0_6 PPP_Printing_Dev;

#endif  


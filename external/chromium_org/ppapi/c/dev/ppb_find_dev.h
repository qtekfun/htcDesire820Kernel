/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_FIND_DEV_H_
#define PPAPI_C_DEV_PPB_FIND_DEV_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_FIND_DEV_INTERFACE_0_3 "PPB_Find(Dev);0.3"
#define PPB_FIND_DEV_INTERFACE PPB_FIND_DEV_INTERFACE_0_3



struct PPB_Find_Dev_0_3 {
  void (*NumberOfFindResultsChanged)(PP_Instance instance,
                                     int32_t total,
                                     PP_Bool final_result);
  void (*SelectedFindResultChanged)(PP_Instance instance, int32_t index);
};

typedef struct PPB_Find_Dev_0_3 PPB_Find_Dev;

#endif  


/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_VIEW_DEV_H_
#define PPAPI_C_DEV_PPB_VIEW_DEV_H_

#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_VIEW_DEV_INTERFACE_0_1 "PPB_View(Dev);0.1"
#define PPB_VIEW_DEV_INTERFACE PPB_VIEW_DEV_INTERFACE_0_1



struct PPB_View_Dev_0_1 {
  float (*GetDeviceScale)(PP_Resource resource);
  float (*GetCSSScale)(PP_Resource resource);
};

typedef struct PPB_View_Dev_0_1 PPB_View_Dev;

#endif  


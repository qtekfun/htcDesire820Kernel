/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_ZOOM_DEV_H_
#define PPAPI_C_DEV_PPB_ZOOM_DEV_H_

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_ZOOM_DEV_INTERFACE_0_2 "PPB_Zoom(Dev);0.2"
#define PPB_ZOOM_DEV_INTERFACE PPB_ZOOM_DEV_INTERFACE_0_2



struct PPB_Zoom_Dev_0_2 {
  void (*ZoomChanged)(PP_Instance instance, double factor);
  void (*ZoomLimitsChanged)(PP_Instance instance,
                            double minimum_factor,
                            double maximum_factor);
};

typedef struct PPB_Zoom_Dev_0_2 PPB_Zoom_Dev;

#endif  


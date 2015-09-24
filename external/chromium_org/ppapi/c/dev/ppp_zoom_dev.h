/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPP_ZOOM_DEV_H_
#define PPAPI_C_DEV_PPP_ZOOM_DEV_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"

#define PPP_ZOOM_DEV_INTERFACE_0_3 "PPP_Zoom(Dev);0.3"
#define PPP_ZOOM_DEV_INTERFACE PPP_ZOOM_DEV_INTERFACE_0_3



struct PPP_Zoom_Dev_0_3 {
  void (*Zoom)(PP_Instance instance, double factor, PP_Bool text_only);
};

typedef struct PPP_Zoom_Dev_0_3 PPP_Zoom_Dev;

#endif  


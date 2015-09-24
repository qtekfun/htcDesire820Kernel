/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_VIEW_H_
#define PPAPI_C_PPB_VIEW_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_point.h"
#include "ppapi/c/pp_rect.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_size.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_VIEW_INTERFACE_1_0 "PPB_View;1.0"
#define PPB_VIEW_INTERFACE_1_1 "PPB_View;1.1"
#define PPB_VIEW_INTERFACE PPB_VIEW_INTERFACE_1_1



struct PPB_View_1_1 {
  PP_Bool (*IsView)(PP_Resource resource);
  PP_Bool (*GetRect)(PP_Resource resource, struct PP_Rect* rect);
  PP_Bool (*IsFullscreen)(PP_Resource resource);
  PP_Bool (*IsVisible)(PP_Resource resource);
  PP_Bool (*IsPageVisible)(PP_Resource resource);
  PP_Bool (*GetClipRect)(PP_Resource resource, struct PP_Rect* clip);
  float (*GetDeviceScale)(PP_Resource resource);
  float (*GetCSSScale)(PP_Resource resource);
};

typedef struct PPB_View_1_1 PPB_View;

struct PPB_View_1_0 {
  PP_Bool (*IsView)(PP_Resource resource);
  PP_Bool (*GetRect)(PP_Resource resource, struct PP_Rect* rect);
  PP_Bool (*IsFullscreen)(PP_Resource resource);
  PP_Bool (*IsVisible)(PP_Resource resource);
  PP_Bool (*IsPageVisible)(PP_Resource resource);
  PP_Bool (*GetClipRect)(PP_Resource resource, struct PP_Rect* clip);
};

#endif  


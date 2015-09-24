/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_WIDGET_DEV_H_
#define PPAPI_C_DEV_PPB_WIDGET_DEV_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_point.h"
#include "ppapi/c/pp_rect.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_size.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_WIDGET_DEV_INTERFACE_0_3 "PPB_Widget(Dev);0.3"
#define PPB_WIDGET_DEV_INTERFACE_0_4 "PPB_Widget(Dev);0.4"
#define PPB_WIDGET_DEV_INTERFACE PPB_WIDGET_DEV_INTERFACE_0_4



struct PPB_Widget_Dev_0_4 {
  PP_Bool (*IsWidget)(PP_Resource resource);
  PP_Bool (*Paint)(PP_Resource widget,
                   const struct PP_Rect* rect,
                   PP_Resource image);
  PP_Bool (*HandleEvent)(PP_Resource widget, PP_Resource input_event);
  PP_Bool (*GetLocation)(PP_Resource widget, struct PP_Rect* location);
  void (*SetLocation)(PP_Resource widget, const struct PP_Rect* location);
  void (*SetScale)(PP_Resource widget, float scale);
};

typedef struct PPB_Widget_Dev_0_4 PPB_Widget_Dev;

struct PPB_Widget_Dev_0_3 {
  PP_Bool (*IsWidget)(PP_Resource resource);
  PP_Bool (*Paint)(PP_Resource widget,
                   const struct PP_Rect* rect,
                   PP_Resource image);
  PP_Bool (*HandleEvent)(PP_Resource widget, PP_Resource input_event);
  PP_Bool (*GetLocation)(PP_Resource widget, struct PP_Rect* location);
  void (*SetLocation)(PP_Resource widget, const struct PP_Rect* location);
};

#endif  


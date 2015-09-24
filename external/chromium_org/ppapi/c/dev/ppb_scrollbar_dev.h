/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_SCROLLBAR_DEV_H_
#define PPAPI_C_DEV_PPB_SCROLLBAR_DEV_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_point.h"
#include "ppapi/c/pp_rect.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_size.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_SCROLLBAR_DEV_INTERFACE_0_5 "PPB_Scrollbar(Dev);0.5"
#define PPB_SCROLLBAR_DEV_INTERFACE PPB_SCROLLBAR_DEV_INTERFACE_0_5



typedef enum {
  PP_SCROLLBY_PIXEL = 0,
  PP_SCROLLBY_LINE = 1,
  PP_SCROLLBY_PAGE = 2,
  PP_SCROLLBY_DOCUMENT = 3
} PP_ScrollBy_Dev;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_ScrollBy_Dev, 4);

struct PPB_Scrollbar_Dev_0_5 {
  PP_Resource (*Create)(PP_Instance instance, PP_Bool vertical);
  PP_Bool (*IsScrollbar)(PP_Resource resource);
  uint32_t (*GetThickness)(PP_Resource resource);
  PP_Bool (*IsOverlay)(PP_Resource scrollbar);
  uint32_t (*GetValue)(PP_Resource scrollbar);
  void (*SetValue)(PP_Resource scrollbar, uint32_t value);
  void (*SetDocumentSize)(PP_Resource scrollbar, uint32_t size);
  void (*SetTickMarks)(PP_Resource scrollbar,
                       const struct PP_Rect tick_marks[],
                       uint32_t count);
  void (*ScrollBy)(PP_Resource scrollbar,
                   PP_ScrollBy_Dev unit,
                   int32_t multiplier);
};

typedef struct PPB_Scrollbar_Dev_0_5 PPB_Scrollbar_Dev;

#endif  


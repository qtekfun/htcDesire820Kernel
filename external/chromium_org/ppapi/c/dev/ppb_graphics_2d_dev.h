/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_GRAPHICS_2D_DEV_H_
#define PPAPI_C_DEV_PPB_GRAPHICS_2D_DEV_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_point.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_GRAPHICS2D_DEV_INTERFACE_0_1 "PPB_Graphics2D(Dev);0.1"
#define PPB_GRAPHICS2D_DEV_INTERFACE_0_2 "PPB_Graphics2D(Dev);0.2"
#define PPB_GRAPHICS2D_DEV_INTERFACE PPB_GRAPHICS2D_DEV_INTERFACE_0_2



typedef enum {
  PP_GRAPHICS2D_DEV_RESIZEMODE_DEFAULT,
  PP_GRAPHICS2D_DEV_RESIZEMODE_STRETCH
} PP_Graphics2D_Dev_ResizeMode;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_Graphics2D_Dev_ResizeMode, 4);

struct PPB_Graphics2D_Dev_0_2 {
  PP_Bool (*SetScale)(PP_Resource resource, float scale);
  float (*GetScale)(PP_Resource resource);
  void (*SetOffset)(PP_Resource resource, const struct PP_Point* offset);
  void (*SetResizeMode)(PP_Resource resource,
                        PP_Graphics2D_Dev_ResizeMode resize_mode);
};

typedef struct PPB_Graphics2D_Dev_0_2 PPB_Graphics2D_Dev;

struct PPB_Graphics2D_Dev_0_1 {
  PP_Bool (*SetScale)(PP_Resource resource, float scale);
  float (*GetScale)(PP_Resource resource);
  void (*SetResizeMode)(PP_Resource resource,
                        PP_Graphics2D_Dev_ResizeMode resize_mode);
};

#endif  


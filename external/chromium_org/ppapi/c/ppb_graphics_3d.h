/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_GRAPHICS_3D_H_
#define PPAPI_C_PPB_GRAPHICS_3D_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_GRAPHICS_3D_INTERFACE_1_0 "PPB_Graphics3D;1.0"
#define PPB_GRAPHICS_3D_INTERFACE PPB_GRAPHICS_3D_INTERFACE_1_0



#include "ppapi/c/pp_graphics_3d.h"

struct PPB_Graphics3D_1_0 {
  int32_t (*GetAttribMaxValue)(PP_Resource instance,
                               int32_t attribute,
                               int32_t* value);
  PP_Resource (*Create)(PP_Instance instance,
                        PP_Resource share_context,
                        const int32_t attrib_list[]);
  PP_Bool (*IsGraphics3D)(PP_Resource resource);
  int32_t (*GetAttribs)(PP_Resource context, int32_t attrib_list[]);
  int32_t (*SetAttribs)(PP_Resource context, const int32_t attrib_list[]);
  int32_t (*GetError)(PP_Resource context);
  int32_t (*ResizeBuffers)(PP_Resource context, int32_t width, int32_t height);
  int32_t (*SwapBuffers)(PP_Resource context,
                         struct PP_CompletionCallback callback);
};

typedef struct PPB_Graphics3D_1_0 PPB_Graphics3D;

#endif  


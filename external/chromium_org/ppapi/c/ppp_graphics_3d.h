/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPP_GRAPHICS_3D_H_
#define PPAPI_C_PPP_GRAPHICS_3D_H_

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"

#define PPP_GRAPHICS_3D_INTERFACE_1_0 "PPP_Graphics_3D;1.0"
#define PPP_GRAPHICS_3D_INTERFACE PPP_GRAPHICS_3D_INTERFACE_1_0



struct PPP_Graphics3D_1_0 {
  void (*Graphics3DContextLost)(PP_Instance instance);
};

typedef struct PPP_Graphics3D_1_0 PPP_Graphics3D;

#endif  


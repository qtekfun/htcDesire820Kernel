/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_FULLSCREEN_H_
#define PPAPI_C_PPB_FULLSCREEN_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_size.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_FULLSCREEN_INTERFACE_1_0 "PPB_Fullscreen;1.0"
#define PPB_FULLSCREEN_INTERFACE PPB_FULLSCREEN_INTERFACE_1_0



struct PPB_Fullscreen_1_0 {
  PP_Bool (*IsFullscreen)(PP_Instance instance);
  PP_Bool (*SetFullscreen)(PP_Instance instance, PP_Bool fullscreen);
  PP_Bool (*GetScreenSize)(PP_Instance instance, struct PP_Size* size);
};

typedef struct PPB_Fullscreen_1_0 PPB_Fullscreen;

#endif  


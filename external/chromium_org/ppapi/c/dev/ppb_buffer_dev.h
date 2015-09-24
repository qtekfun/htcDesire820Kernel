/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_BUFFER_DEV_H_
#define PPAPI_C_DEV_PPB_BUFFER_DEV_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_BUFFER_DEV_INTERFACE_0_4 "PPB_Buffer(Dev);0.4"
#define PPB_BUFFER_DEV_INTERFACE PPB_BUFFER_DEV_INTERFACE_0_4



struct PPB_Buffer_Dev_0_4 {
  PP_Resource (*Create)(PP_Instance instance, uint32_t size_in_bytes);
  PP_Bool (*IsBuffer)(PP_Resource resource);
  PP_Bool (*Describe)(PP_Resource resource, uint32_t* size_in_bytes);
  void* (*Map)(PP_Resource resource);
  void (*Unmap)(PP_Resource resource);
};

typedef struct PPB_Buffer_Dev_0_4 PPB_Buffer_Dev;

#endif  


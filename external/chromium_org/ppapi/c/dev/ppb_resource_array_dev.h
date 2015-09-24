/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_RESOURCE_ARRAY_DEV_H_
#define PPAPI_C_DEV_PPB_RESOURCE_ARRAY_DEV_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_RESOURCEARRAY_DEV_INTERFACE_0_1 "PPB_ResourceArray(Dev);0.1"
#define PPB_RESOURCEARRAY_DEV_INTERFACE PPB_RESOURCEARRAY_DEV_INTERFACE_0_1



struct PPB_ResourceArray_Dev_0_1 {
  PP_Resource (*Create)(PP_Instance instance,
                        const PP_Resource elements[],
                        uint32_t size);
  PP_Bool (*IsResourceArray)(PP_Resource resource);
  uint32_t (*GetSize)(PP_Resource resource_array);
  PP_Resource (*GetAt)(PP_Resource resource_array, uint32_t index);
};

typedef struct PPB_ResourceArray_Dev_0_1 PPB_ResourceArray_Dev;

#endif  


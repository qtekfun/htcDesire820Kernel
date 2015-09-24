/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_DEVICE_REF_DEV_H_
#define PPAPI_C_DEV_PPB_DEVICE_REF_DEV_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_DEVICEREF_DEV_INTERFACE_0_1 "PPB_DeviceRef(Dev);0.1"
#define PPB_DEVICEREF_DEV_INTERFACE PPB_DEVICEREF_DEV_INTERFACE_0_1



typedef void (*PP_MonitorDeviceChangeCallback)(void* user_data,
                                               uint32_t device_count,
                                               const PP_Resource devices[]);

typedef enum {
  PP_DEVICETYPE_DEV_INVALID = 0,
  PP_DEVICETYPE_DEV_AUDIOCAPTURE = 1,
  PP_DEVICETYPE_DEV_VIDEOCAPTURE = 2
} PP_DeviceType_Dev;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_DeviceType_Dev, 4);

struct PPB_DeviceRef_Dev_0_1 {
  PP_Bool (*IsDeviceRef)(PP_Resource resource);
  PP_DeviceType_Dev (*GetType)(PP_Resource device_ref);
  struct PP_Var (*GetName)(PP_Resource device_ref);
};

typedef struct PPB_DeviceRef_Dev_0_1 PPB_DeviceRef_Dev;

#endif  


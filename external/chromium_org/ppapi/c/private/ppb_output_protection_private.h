/* Copyright 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPB_OUTPUT_PROTECTION_PRIVATE_H_
#define PPAPI_C_PRIVATE_PPB_OUTPUT_PROTECTION_PRIVATE_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_OUTPUTPROTECTION_PRIVATE_INTERFACE_0_1 \
    "PPB_OutputProtection_Private;0.1"
#define PPB_OUTPUTPROTECTION_PRIVATE_INTERFACE \
    PPB_OUTPUTPROTECTION_PRIVATE_INTERFACE_0_1



typedef enum {
  PP_OUTPUT_PROTECTION_METHOD_PRIVATE_NONE = 0,
  PP_OUTPUT_PROTECTION_METHOD_PRIVATE_HDCP = 1 << 0
} PP_OutputProtectionMethod_Private;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_OutputProtectionMethod_Private, 4);

typedef enum {
  PP_OUTPUT_PROTECTION_LINK_TYPE_PRIVATE_NONE = 0,
  PP_OUTPUT_PROTECTION_LINK_TYPE_PRIVATE_UNKNOWN = 1 << 0,
  PP_OUTPUT_PROTECTION_LINK_TYPE_PRIVATE_INTERNAL = 1 << 1,
  PP_OUTPUT_PROTECTION_LINK_TYPE_PRIVATE_VGA = 1 << 2,
  PP_OUTPUT_PROTECTION_LINK_TYPE_PRIVATE_HDMI = 1 << 3,
  PP_OUTPUT_PROTECTION_LINK_TYPE_PRIVATE_DVI = 1 << 4,
  PP_OUTPUT_PROTECTION_LINK_TYPE_PRIVATE_DISPLAYPORT = 1 << 5,
  PP_OUTPUT_PROTECTION_LINK_TYPE_PRIVATE_NETWORK = 1 << 6
} PP_OutputProtectionLinkType_Private;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_OutputProtectionLinkType_Private, 4);

struct PPB_OutputProtection_Private_0_1 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsOutputProtection)(PP_Resource resource);
  int32_t (*QueryStatus)(PP_Resource resource,
                         uint32_t* link_mask,
                         uint32_t* protection_mask,
                         struct PP_CompletionCallback callback);
  int32_t (*EnableProtection)(PP_Resource resource,
                              uint32_t desired_protection_mask,
                              struct PP_CompletionCallback callback);
};

typedef struct PPB_OutputProtection_Private_0_1 PPB_OutputProtection_Private;

#endif  


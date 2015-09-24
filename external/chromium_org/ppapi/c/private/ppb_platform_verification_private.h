/* Copyright 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPB_PLATFORM_VERIFICATION_PRIVATE_H_
#define PPAPI_C_PRIVATE_PPB_PLATFORM_VERIFICATION_PRIVATE_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_PLATFORMVERIFICATION_PRIVATE_INTERFACE_0_2 \
    "PPB_PlatformVerification_Private;0.2"
#define PPB_PLATFORMVERIFICATION_PRIVATE_INTERFACE \
    PPB_PLATFORMVERIFICATION_PRIVATE_INTERFACE_0_2



struct PPB_PlatformVerification_Private_0_2 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsPlatformVerification)(PP_Resource resource);
  int32_t (*ChallengePlatform)(PP_Resource instance,
                               struct PP_Var service_id,
                               struct PP_Var challenge,
                               struct PP_Var* signed_data,
                               struct PP_Var* signed_data_signature,
                               struct PP_Var* platform_key_certificate,
                               struct PP_CompletionCallback callback);
};

typedef struct PPB_PlatformVerification_Private_0_2
    PPB_PlatformVerification_Private;

#endif  


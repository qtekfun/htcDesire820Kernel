/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_TRUSTED_PPB_BROKER_TRUSTED_H_
#define PPAPI_C_TRUSTED_PPB_BROKER_TRUSTED_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_BROKER_TRUSTED_INTERFACE_0_2 "PPB_BrokerTrusted;0.2"
#define PPB_BROKER_TRUSTED_INTERFACE_0_3 "PPB_BrokerTrusted;0.3"
#define PPB_BROKER_TRUSTED_INTERFACE PPB_BROKER_TRUSTED_INTERFACE_0_3



struct PPB_BrokerTrusted_0_3 {
  PP_Resource (*CreateTrusted)(PP_Instance instance);
  PP_Bool (*IsBrokerTrusted)(PP_Resource resource);
  int32_t (*Connect)(PP_Resource broker,
                     struct PP_CompletionCallback connect_callback);
  int32_t (*GetHandle)(PP_Resource broker, int32_t* handle);
  PP_Bool (*IsAllowed)(PP_Resource broker);
};

typedef struct PPB_BrokerTrusted_0_3 PPB_BrokerTrusted;

struct PPB_BrokerTrusted_0_2 {
  PP_Resource (*CreateTrusted)(PP_Instance instance);
  PP_Bool (*IsBrokerTrusted)(PP_Resource resource);
  int32_t (*Connect)(PP_Resource broker,
                     struct PP_CompletionCallback connect_callback);
  int32_t (*GetHandle)(PP_Resource broker, int32_t* handle);
};

#endif  


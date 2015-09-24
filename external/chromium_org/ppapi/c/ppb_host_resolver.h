/* Copyright 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_HOST_RESOLVER_H_
#define PPAPI_C_PPB_HOST_RESOLVER_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/c/ppb_net_address.h"

#define PPB_HOSTRESOLVER_INTERFACE_1_0 "PPB_HostResolver;1.0"
#define PPB_HOSTRESOLVER_INTERFACE PPB_HOSTRESOLVER_INTERFACE_1_0



typedef enum {
  PP_HOSTRESOLVER_FLAG_CANONNAME = 1 << 0
} PP_HostResolver_Flag;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_HostResolver_Flag, 4);

struct PP_HostResolver_Hint {
  PP_NetAddress_Family family;
  int32_t flags;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_HostResolver_Hint, 8);

struct PPB_HostResolver_1_0 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsHostResolver)(PP_Resource resource);
  int32_t (*Resolve)(PP_Resource host_resolver,
                     const char* host,
                     uint16_t port,
                     const struct PP_HostResolver_Hint* hint,
                     struct PP_CompletionCallback callback);
  struct PP_Var (*GetCanonicalName)(PP_Resource host_resolver);
  uint32_t (*GetNetAddressCount)(PP_Resource host_resolver);
  PP_Resource (*GetNetAddress)(PP_Resource host_resolver, uint32_t index);
};

typedef struct PPB_HostResolver_1_0 PPB_HostResolver;

#endif  


/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPB_HOST_RESOLVER_PRIVATE_H_
#define PPAPI_C_PRIVATE_PPB_HOST_RESOLVER_PRIVATE_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/c/private/ppb_net_address_private.h"

#define PPB_HOSTRESOLVER_PRIVATE_INTERFACE_0_1 "PPB_HostResolver_Private;0.1"
#define PPB_HOSTRESOLVER_PRIVATE_INTERFACE \
    PPB_HOSTRESOLVER_PRIVATE_INTERFACE_0_1



typedef enum {
  PP_HOST_RESOLVER_PRIVATE_FLAGS_CANONNAME = 1 << 0,
  PP_HOST_RESOLVER_PRIVATE_FLAGS_LOOPBACK_ONLY = 1 << 1
} PP_HostResolver_Private_Flags;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_HostResolver_Private_Flags, 4);

struct PP_HostResolver_Private_Hint {
  PP_NetAddressFamily_Private family;
  int32_t flags;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_HostResolver_Private_Hint, 8);

struct PPB_HostResolver_Private_0_1 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsHostResolver)(PP_Resource resource);
  int32_t (*Resolve)(PP_Resource host_resolver,
                     const char* host,
                     uint16_t port,
                     const struct PP_HostResolver_Private_Hint* hint,
                     struct PP_CompletionCallback callback);
  struct PP_Var (*GetCanonicalName)(PP_Resource host_resolver);
  uint32_t (*GetSize)(PP_Resource host_resolver);
  PP_Bool (*GetNetAddress)(PP_Resource host_resolver,
                           uint32_t index,
                           struct PP_NetAddress_Private* addr);
};

typedef struct PPB_HostResolver_Private_0_1 PPB_HostResolver_Private;

#endif  


/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_TRUSTED_PPB_URL_LOADER_TRUSTED_H_
#define PPAPI_C_TRUSTED_PPB_URL_LOADER_TRUSTED_H_

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_URLLOADERTRUSTED_INTERFACE_0_3 "PPB_URLLoaderTrusted;0.3"
#define PPB_URLLOADERTRUSTED_INTERFACE PPB_URLLOADERTRUSTED_INTERFACE_0_3



typedef void (*PP_URLLoaderTrusted_StatusCallback)(
    PP_Instance pp_instance,
    PP_Resource pp_resource,
    int64_t bytes_sent,
    int64_t total_bytes_to_be_sent,
    int64_t bytes_received,
    int64_t total_bytes_to_be_received);

struct PPB_URLLoaderTrusted_0_3 {
  void (*GrantUniversalAccess)(PP_Resource loader);
  void (*RegisterStatusCallback)(PP_Resource loader,
                                 PP_URLLoaderTrusted_StatusCallback cb);
};

typedef struct PPB_URLLoaderTrusted_0_3 PPB_URLLoaderTrusted;

#endif  


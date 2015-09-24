/* Copyright 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_NETWORK_PROXY_H_
#define PPAPI_C_PPB_NETWORK_PROXY_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_NETWORKPROXY_INTERFACE_1_0 "PPB_NetworkProxy;1.0"
#define PPB_NETWORKPROXY_INTERFACE PPB_NETWORKPROXY_INTERFACE_1_0



struct PPB_NetworkProxy_1_0 {
  int32_t (*GetProxyForURL)(PP_Instance instance,
                            struct PP_Var url,
                            struct PP_Var* proxy_string,
                            struct PP_CompletionCallback callback);
};

typedef struct PPB_NetworkProxy_1_0 PPB_NetworkProxy;

#endif  


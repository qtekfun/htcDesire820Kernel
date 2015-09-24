/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_NETWORK_MONITOR_H_
#define PPAPI_C_PPB_NETWORK_MONITOR_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_NETWORKMONITOR_INTERFACE_1_0 "PPB_NetworkMonitor;1.0"
#define PPB_NETWORKMONITOR_INTERFACE PPB_NETWORKMONITOR_INTERFACE_1_0



struct PPB_NetworkMonitor_1_0 {
  PP_Resource (*Create)(PP_Instance instance);
  int32_t (*UpdateNetworkList)(PP_Resource network_monitor,
                               PP_Resource* network_list,
                               struct PP_CompletionCallback callback);
  PP_Bool (*IsNetworkMonitor)(PP_Resource resource);
};

typedef struct PPB_NetworkMonitor_1_0 PPB_NetworkMonitor;

#endif  


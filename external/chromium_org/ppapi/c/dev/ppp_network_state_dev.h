/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPP_NETWORK_STATE_DEV_H_
#define PPAPI_C_DEV_PPP_NETWORK_STATE_DEV_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"

#define PPP_NETWORK_STATE_DEV_INTERFACE_0_1 "PPP_NetworkState(Dev);0.1"
#define PPP_NETWORK_STATE_DEV_INTERFACE PPP_NETWORK_STATE_DEV_INTERFACE_0_1



struct PPP_NetworkState_Dev_0_1 {
  void (*SetOnLine)(PP_Bool is_online);
};

typedef struct PPP_NetworkState_Dev_0_1 PPP_NetworkState_Dev;

#endif  


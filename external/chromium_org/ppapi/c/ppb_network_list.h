/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_NETWORK_LIST_H_
#define PPAPI_C_PPB_NETWORK_LIST_H_

#include "ppapi/c/pp_array_output.h"
#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_NETWORKLIST_INTERFACE_1_0 "PPB_NetworkList;1.0"
#define PPB_NETWORKLIST_INTERFACE PPB_NETWORKLIST_INTERFACE_1_0



typedef enum {
  PP_NETWORKLIST_TYPE_UNKNOWN = 0,
  PP_NETWORKLIST_TYPE_ETHERNET = 1,
  PP_NETWORKLIST_TYPE_WIFI = 2,
  PP_NETWORKLIST_TYPE_CELLULAR = 3
} PP_NetworkList_Type;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_NetworkList_Type, 4);

typedef enum {
  PP_NETWORKLIST_STATE_DOWN = 0,
  PP_NETWORKLIST_STATE_UP = 1
} PP_NetworkList_State;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_NetworkList_State, 4);

struct PPB_NetworkList_1_0 {
  PP_Bool (*IsNetworkList)(PP_Resource resource);
  uint32_t (*GetCount)(PP_Resource resource);
  struct PP_Var (*GetName)(PP_Resource resource, uint32_t index);
  PP_NetworkList_Type (*GetType)(PP_Resource resource, uint32_t index);
  PP_NetworkList_State (*GetState)(PP_Resource resource, uint32_t index);
  int32_t (*GetIpAddresses)(PP_Resource resource,
                            uint32_t index,
                            struct PP_ArrayOutput output);
  struct PP_Var (*GetDisplayName)(PP_Resource resource, uint32_t index);
  uint32_t (*GetMTU)(PP_Resource resource, uint32_t index);
};

typedef struct PPB_NetworkList_1_0 PPB_NetworkList;

#endif  


/* Copyright (c) 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPB_EXT_CRX_FILE_SYSTEM_PRIVATE_H_
#define PPAPI_C_PRIVATE_PPB_EXT_CRX_FILE_SYSTEM_PRIVATE_H_

#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_EXT_CRXFILESYSTEM_PRIVATE_INTERFACE_0_1 \
    "PPB_Ext_CrxFileSystem_Private;0.1"
#define PPB_EXT_CRXFILESYSTEM_PRIVATE_INTERFACE \
    PPB_EXT_CRXFILESYSTEM_PRIVATE_INTERFACE_0_1



struct PPB_Ext_CrxFileSystem_Private_0_1 {
  int32_t (*Open)(PP_Instance instance,
                  PP_Resource* file_system,
                  struct PP_CompletionCallback callback);
};

typedef struct PPB_Ext_CrxFileSystem_Private_0_1 PPB_Ext_CrxFileSystem_Private;

#endif  


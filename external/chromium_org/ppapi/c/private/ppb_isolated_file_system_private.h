/* Copyright 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPB_ISOLATED_FILE_SYSTEM_PRIVATE_H_
#define PPAPI_C_PRIVATE_PPB_ISOLATED_FILE_SYSTEM_PRIVATE_H_

#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_ISOLATEDFILESYSTEM_PRIVATE_INTERFACE_0_2 \
    "PPB_IsolatedFileSystem_Private;0.2"
#define PPB_ISOLATEDFILESYSTEM_PRIVATE_INTERFACE \
    PPB_ISOLATEDFILESYSTEM_PRIVATE_INTERFACE_0_2



typedef enum {
  
  PP_ISOLATEDFILESYSTEMTYPE_PRIVATE_INVALID = 0,
  
  PP_ISOLATEDFILESYSTEMTYPE_PRIVATE_CRX = 1,
  
  PP_ISOLATEDFILESYSTEMTYPE_PRIVATE_PLUGINPRIVATE = 2
} PP_IsolatedFileSystemType_Private;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_IsolatedFileSystemType_Private, 4);

struct PPB_IsolatedFileSystem_Private_0_2 {
  int32_t (*Open)(PP_Instance instance,
                  PP_IsolatedFileSystemType_Private type,
                  PP_Resource* file_system,
                  struct PP_CompletionCallback callback);
};

typedef struct PPB_IsolatedFileSystem_Private_0_2
    PPB_IsolatedFileSystem_Private;

#endif  


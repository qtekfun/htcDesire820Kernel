/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_FILE_SYSTEM_H_
#define PPAPI_C_PPB_FILE_SYSTEM_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_file_info.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_FILESYSTEM_INTERFACE_1_0 "PPB_FileSystem;1.0"
#define PPB_FILESYSTEM_INTERFACE PPB_FILESYSTEM_INTERFACE_1_0



struct PPB_FileSystem_1_0 {
  PP_Resource (*Create)(PP_Instance instance, PP_FileSystemType type);
  PP_Bool (*IsFileSystem)(PP_Resource resource);
  int32_t (*Open)(PP_Resource file_system,
                  int64_t expected_size,
                  struct PP_CompletionCallback callback);
  PP_FileSystemType (*GetType)(PP_Resource file_system);
};

typedef struct PPB_FileSystem_1_0 PPB_FileSystem;

#endif  


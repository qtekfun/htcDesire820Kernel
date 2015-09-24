/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PP_FILE_INFO_H_
#define PPAPI_C_PP_FILE_INFO_H_

#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_time.h"



typedef enum {
  
  PP_FILETYPE_REGULAR = 0,
  
  PP_FILETYPE_DIRECTORY = 1,
  
  PP_FILETYPE_OTHER = 2
} PP_FileType;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_FileType, 4);

typedef enum {
  
  PP_FILESYSTEMTYPE_INVALID = 0,
  
  PP_FILESYSTEMTYPE_EXTERNAL = 1,
  
  PP_FILESYSTEMTYPE_LOCALPERSISTENT = 2,
  
  PP_FILESYSTEMTYPE_LOCALTEMPORARY = 3,
  
  PP_FILESYSTEMTYPE_ISOLATED = 4
} PP_FileSystemType;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_FileSystemType, 4);

struct PP_FileInfo {
  
  int64_t size;
  PP_FileType type;
  PP_FileSystemType system_type;
  PP_Time creation_time;
  PP_Time last_access_time;
  PP_Time last_modified_time;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_FileInfo, 40);

#endif  


/* Copyright (c) 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PP_DIRECTORY_ENTRY_H_
#define PPAPI_C_PP_DIRECTORY_ENTRY_H_

#include "ppapi/c/pp_file_info.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"



struct PP_DirectoryEntry {
  PP_Resource file_ref;
  PP_FileType file_type;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_DirectoryEntry, 8);

#endif  


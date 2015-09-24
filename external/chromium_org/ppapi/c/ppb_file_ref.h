/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_FILE_REF_H_
#define PPAPI_C_PPB_FILE_REF_H_

#include "ppapi/c/pp_array_output.h"
#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_file_info.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_time.h"
#include "ppapi/c/pp_var.h"

#define PPB_FILEREF_INTERFACE_1_0 "PPB_FileRef;1.0"
#define PPB_FILEREF_INTERFACE_1_1 "PPB_FileRef;1.1"
#define PPB_FILEREF_INTERFACE PPB_FILEREF_INTERFACE_1_1



struct PPB_FileRef_1_1 {
  PP_Resource (*Create)(PP_Resource file_system, const char* path);
  PP_Bool (*IsFileRef)(PP_Resource resource);
  PP_FileSystemType (*GetFileSystemType)(PP_Resource file_ref);
  struct PP_Var (*GetName)(PP_Resource file_ref);
  struct PP_Var (*GetPath)(PP_Resource file_ref);
  PP_Resource (*GetParent)(PP_Resource file_ref);
  int32_t (*MakeDirectory)(PP_Resource directory_ref,
                           PP_Bool make_ancestors,
                           struct PP_CompletionCallback callback);
  int32_t (*Touch)(PP_Resource file_ref,
                   PP_Time last_access_time,
                   PP_Time last_modified_time,
                   struct PP_CompletionCallback callback);
  int32_t (*Delete)(PP_Resource file_ref,
                    struct PP_CompletionCallback callback);
  int32_t (*Rename)(PP_Resource file_ref,
                    PP_Resource new_file_ref,
                    struct PP_CompletionCallback callback);
  int32_t (*Query)(PP_Resource file_ref,
                   struct PP_FileInfo* info,
                   struct PP_CompletionCallback callback);
  int32_t (*ReadDirectoryEntries)(PP_Resource file_ref,
                                  struct PP_ArrayOutput output,
                                  struct PP_CompletionCallback callback);
};

typedef struct PPB_FileRef_1_1 PPB_FileRef;

struct PPB_FileRef_1_0 {
  PP_Resource (*Create)(PP_Resource file_system, const char* path);
  PP_Bool (*IsFileRef)(PP_Resource resource);
  PP_FileSystemType (*GetFileSystemType)(PP_Resource file_ref);
  struct PP_Var (*GetName)(PP_Resource file_ref);
  struct PP_Var (*GetPath)(PP_Resource file_ref);
  PP_Resource (*GetParent)(PP_Resource file_ref);
  int32_t (*MakeDirectory)(PP_Resource directory_ref,
                           PP_Bool make_ancestors,
                           struct PP_CompletionCallback callback);
  int32_t (*Touch)(PP_Resource file_ref,
                   PP_Time last_access_time,
                   PP_Time last_modified_time,
                   struct PP_CompletionCallback callback);
  int32_t (*Delete)(PP_Resource file_ref,
                    struct PP_CompletionCallback callback);
  int32_t (*Rename)(PP_Resource file_ref,
                    PP_Resource new_file_ref,
                    struct PP_CompletionCallback callback);
};

#endif  


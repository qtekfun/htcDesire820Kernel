// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PRIVATE_PPB_FLASH_FILE_H_
#define PPAPI_C_PRIVATE_PPB_FLASH_FILE_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/private/pp_file_handle.h"

struct PP_FileInfo;

struct PP_DirEntry_Dev {
  const char* name;
  PP_Bool is_dir;
};

struct PP_DirContents_Dev {
  int32_t count;
  struct PP_DirEntry_Dev* entries;
};

#define PPB_FLASH_FILE_MODULELOCAL_INTERFACE_3_0 "PPB_Flash_File_ModuleLocal;3"
#define PPB_FLASH_FILE_MODULELOCAL_INTERFACE \
    PPB_FLASH_FILE_MODULELOCAL_INTERFACE_3_0

struct PPB_Flash_File_ModuleLocal_3_0 {
  
  bool (*CreateThreadAdapterForInstance)(PP_Instance instance);
  
  void (*ClearThreadAdapterForInstance)(PP_Instance instance);

  
  
  
  int32_t (*OpenFile)(PP_Instance instance,
                      const char* path,
                      int32_t mode,
                      PP_FileHandle* file);

  
  
  int32_t (*RenameFile)(PP_Instance instance,
                        const char* path_from,
                        const char* path_to);

  
  
  
  
  int32_t (*DeleteFileOrDir)(PP_Instance instance,
                             const char* path,
                             PP_Bool recursive);

  
  
  int32_t (*CreateDir)(PP_Instance instance, const char* path);

  
  
  int32_t (*QueryFile)(PP_Instance instance,
                       const char* path,
                       struct PP_FileInfo* info);

  
  
  
  int32_t (*GetDirContents)(PP_Instance instance,
                            const char* path,
                            struct PP_DirContents_Dev** contents);

  
  void (*FreeDirContents)(PP_Instance instance,
                          struct PP_DirContents_Dev* contents);

  
  
  
  
  
  
  
  int32_t (*CreateTemporaryFile)(PP_Instance instance, PP_FileHandle* file);
};

typedef struct PPB_Flash_File_ModuleLocal_3_0 PPB_Flash_File_ModuleLocal;


#define PPB_FLASH_FILE_FILEREF_INTERFACE "PPB_Flash_File_FileRef;2"

struct PPB_Flash_File_FileRef {
  
  
  
  int32_t (*OpenFile)(PP_Resource file_ref_id,
                      int32_t mode,
                      PP_FileHandle* file);
  int32_t (*QueryFile)(PP_Resource file_ref_id,
                       struct PP_FileInfo* info);
};

#endif  

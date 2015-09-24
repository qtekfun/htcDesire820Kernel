// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_FILE_REF_CREATE_INFO_H
#define PPAPI_SHARED_IMPL_FILE_REF_CREATE_INFO_H

#include <string>

#include "base/files/file_path.h"
#include "ppapi/c/pp_file_info.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/shared_impl/ppapi_shared_export.h"

namespace ppapi {

struct FileRefCreateInfo {
  FileRefCreateInfo() : file_system_type(PP_FILESYSTEMTYPE_INVALID),
                        browser_pending_host_resource_id(0),
                        renderer_pending_host_resource_id(0),
                        file_system_plugin_resource(0) { }

  PPAPI_SHARED_EXPORT bool IsValid() const;

  PP_FileSystemType file_system_type;
  std::string internal_path;
  std::string display_name;

  
  int browser_pending_host_resource_id;
  int renderer_pending_host_resource_id;

  
  
  
  PP_Resource file_system_plugin_resource;
};

PPAPI_SHARED_EXPORT FileRefCreateInfo
MakeExternalFileRefCreateInfo(const base::FilePath& external_path,
                              const std::string& display_name,
                              int browser_pending_host_resource_id,
                              int renderer_pending_host_resource_id);

}  

#endif  

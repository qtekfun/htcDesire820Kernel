// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_FILE_SYSTEM_H_
#define PPAPI_CPP_FILE_SYSTEM_H_

#include "ppapi/c/pp_file_info.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_time.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/resource.h"


namespace pp {

class CompletionCallback;

class FileSystem : public Resource {
 public:
  
  
  
  FileSystem();

  
  
  
  FileSystem(const FileSystem& other);

  
  
  
  explicit FileSystem(const Resource& resource);

  
  
  
  
  FileSystem(PassRef, PP_Resource resource);

  
  
  
  
  
  
  
  FileSystem(const InstanceHandle& instance, PP_FileSystemType type);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t Open(int64_t expected_size, const CompletionCallback& cc);

  
  
  
  
  
  
  static bool IsFileSystem(const Resource& resource);
};

}  

#endif  

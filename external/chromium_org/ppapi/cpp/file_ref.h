// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_FILE_REF_H_
#define PPAPI_CPP_FILE_REF_H_

#include "ppapi/c/pp_file_info.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/ppb_file_ref.h"
#include "ppapi/cpp/resource.h"
#include "ppapi/cpp/var.h"


namespace pp {

class DirectoryEntry;
class FileSystem;
class CompletionCallback;
template <typename T> class CompletionCallbackWithOutput;

class FileRef : public Resource {
 public:
  
  
  FileRef() {}

  
  
  
  
  
  explicit FileRef(PP_Resource resource);

  
  
  
  
  FileRef(PassRef, PP_Resource resource);

  
  
  
  
  
  
  
  
  
  FileRef(const FileSystem& file_system, const char* path);

  
  
  
  FileRef(const FileRef& other);

  
  
  
  
  
  PP_FileSystemType GetFileSystemType() const;

  
  
  
  
  
  
  Var GetName() const;

  
  
  
  
  
  Var GetPath() const;

  
  
  
  
  
  
  
  FileRef GetParent() const;

  
  
  
  
  
  
  
  
  
  
  
  
  int32_t MakeDirectory(const CompletionCallback& cc);

  
  
  
  
  
  
  
  
  
  int32_t MakeDirectoryIncludingAncestors(const CompletionCallback& cc);

  
  
  
  
  
  
  
  
  
  int32_t Touch(PP_Time last_access_time,
                PP_Time last_modified_time,
                const CompletionCallback& cc);

  
  
  
  
  
  
  
  
  
  int32_t Delete(const CompletionCallback& cc);

  
  
  
  
  
  
  
  
  
  
  
  int32_t Rename(const FileRef& new_file_ref, const CompletionCallback& cc);

  
  
  
  
  
  
  
  int32_t Query(const CompletionCallbackWithOutput<PP_FileInfo>& callback);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t ReadDirectoryEntries(
      const CompletionCallbackWithOutput< std::vector<DirectoryEntry> >&
          callback);
};

}  

#endif  

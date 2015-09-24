// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_DIRECTORY_ENTRY_H_
#define PPAPI_CPP_DIRECTORY_ENTRY_H_

#include <vector>

#include "ppapi/c/pp_array_output.h"
#include "ppapi/c/pp_directory_entry.h"
#include "ppapi/cpp/array_output.h"
#include "ppapi/cpp/file_ref.h"
#include "ppapi/cpp/output_traits.h"
#include "ppapi/cpp/pass_ref.h"


namespace pp {

class DirectoryEntry {
 public:
  
  
  DirectoryEntry();

  
  
  
  
  
  DirectoryEntry(PassRef, const PP_DirectoryEntry& data);

  
  
  
  
  
  DirectoryEntry(const DirectoryEntry& other);

  
  
  ~DirectoryEntry();

  
  
  
  
  
  
  
  
  DirectoryEntry& operator=(const DirectoryEntry& other);

  
  
  
  
  bool is_null() const { return !data_.file_ref; }

  
  
  
  
  FileRef file_ref() const { return FileRef(data_.file_ref); }

  
  
  
  
  PP_FileType file_type() const { return data_.file_type; }

 private:
  PP_DirectoryEntry data_;
};

namespace internal {

class DirectoryEntryArrayOutputAdapterWithStorage
    : public ArrayOutputAdapter<PP_DirectoryEntry> {
 public:
  DirectoryEntryArrayOutputAdapterWithStorage();
  virtual ~DirectoryEntryArrayOutputAdapterWithStorage();

  
  // PP_DirectoryEntry written by the browser to pp::DirectoryEntry
  
  
  
  
  std::vector<DirectoryEntry>& output();

 private:
  
  std::vector<PP_DirectoryEntry> temp_storage_;

  
  
  
  std::vector<DirectoryEntry> output_storage_;
};

template <>
struct CallbackOutputTraits< std::vector<DirectoryEntry> > {
  typedef PP_ArrayOutput APIArgType;
  typedef DirectoryEntryArrayOutputAdapterWithStorage StorageType;

  static inline APIArgType StorageToAPIArg(StorageType& t) {
    return t.pp_array_output();
  }

  static inline std::vector<DirectoryEntry>& StorageToPluginArg(
      StorageType& t) {
    return t.output();
  }

  static inline void Initialize(StorageType* ) {}
};

}  
}  

#endif  

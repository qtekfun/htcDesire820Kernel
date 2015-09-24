// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_FILES_MEMORY_MAPPED_FILE_H_
#define BASE_FILES_MEMORY_MAPPED_FILE_H_

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/platform_file.h"
#include "build/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#endif

namespace base {

class FilePath;

class BASE_EXPORT MemoryMappedFile {
 public:
  
  MemoryMappedFile();
  ~MemoryMappedFile();

  
  
  
  
  
  bool Initialize(const FilePath& file_name);
  
  
  bool Initialize(PlatformFile file);

#if defined(OS_WIN)
  
  
  bool InitializeAsImageSection(const FilePath& file_name);
#endif  

  const uint8* data() const { return data_; }
  size_t length() const { return length_; }

  
  bool IsValid() const;

 private:
  
  bool MapFileToMemory(const FilePath& file_name);

  
  
  bool MapFileToMemoryInternal();

  
  void CloseHandles();

#if defined(OS_WIN)
  
  
  bool MapFileToMemoryInternalEx(int flags);

  HANDLE file_mapping_;
#endif
  PlatformFile file_;
  uint8* data_;
  size_t length_;

  DISALLOW_COPY_AND_ASSIGN(MemoryMappedFile);
};

}  

#endif  

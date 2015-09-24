// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_DISK_CACHE_MAPPED_FILE_H_
#define NET_DISK_CACHE_MAPPED_FILE_H_
#pragma once

#include "net/disk_cache/disk_format.h"
#include "net/disk_cache/file.h"
#include "net/disk_cache/file_block.h"

class FilePath;

namespace disk_cache {

class MappedFile : public File {
 public:
  MappedFile() : File(true), init_(false) {}

  
  
  
  void* Init(const FilePath& name, size_t size);

  void* buffer() const {
    return buffer_;
  }

  
  bool Load(const FileBlock* block);
  bool Store(const FileBlock* block);

 private:
  virtual ~MappedFile();

  bool init_;
#if defined(OS_WIN)
  HANDLE section_;
#endif
  void* buffer_;  
  size_t view_size_;  

  DISALLOW_COPY_AND_ASSIGN(MappedFile);
};

}  

#endif  

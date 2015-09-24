// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_DISK_CACHE_FILE_H_
#define NET_DISK_CACHE_FILE_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "base/platform_file.h"

class FilePath;

namespace disk_cache {

class FileIOCallback {
 public:
  virtual ~FileIOCallback() {}

  // Notified of the actual number of bytes read or written. This value is
  
  virtual void OnFileIOComplete(int bytes_copied) = 0;
};

class File : public base::RefCounted<File> {
  friend class base::RefCounted<File>;
 public:
  File();
  
  explicit File(bool mixed_mode);

  
  
  
  explicit File(base::PlatformFile file);

  
  
  bool Init(const FilePath& name);

  
  base::PlatformFile platform_file() const;

  
  bool IsValid() const;

  
  bool Read(void* buffer, size_t buffer_len, size_t offset);
  bool Write(const void* buffer, size_t buffer_len, size_t offset);

  
  
  bool Read(void* buffer, size_t buffer_len, size_t offset,
            FileIOCallback* callback, bool* completed);
  bool Write(const void* buffer, size_t buffer_len, size_t offset,
             FileIOCallback* callback, bool* completed);

  
  
  bool SetLength(size_t length);
  size_t GetLength();

  
  static void WaitForPendingIO(int* num_pending_io);

 protected:
  virtual ~File();

  
  
  bool AsyncWrite(const void* buffer, size_t buffer_len, size_t offset,
                  FileIOCallback* callback, bool* completed);

 private:
  bool init_;
  bool mixed_;
  base::PlatformFile platform_file_;  
  base::PlatformFile sync_platform_file_;  

  DISALLOW_COPY_AND_ASSIGN(File);
};

}  

#endif  

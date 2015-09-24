// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_DISK_CACHE_FILE_H_
#define NET_DISK_CACHE_FILE_H_

#include "base/memory/ref_counted.h"
#include "base/platform_file.h"
#include "net/base/net_export.h"

namespace base {
class FilePath;
}

namespace disk_cache {

class FileIOCallback {
 public:
  // Notified of the actual number of bytes read or written. This value is
  
  virtual void OnFileIOComplete(int bytes_copied) = 0;

 protected:
  virtual ~FileIOCallback() {}
};

class NET_EXPORT_PRIVATE File : public base::RefCounted<File> {
  friend class base::RefCounted<File>;
 public:
  File();
  
  explicit File(bool mixed_mode);

  
  
  
  explicit File(base::PlatformFile file);

  
  
  bool Init(const base::FilePath& name);

  
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

  
  static void DropPendingIO();

 protected:
  virtual ~File();

 private:
  
  
  bool AsyncWrite(const void* buffer, size_t buffer_len, size_t offset,
                  FileIOCallback* callback, bool* completed);

  
  int DoRead(void* buffer, size_t buffer_len, size_t offset);
  int DoWrite(const void* buffer, size_t buffer_len, size_t offset);
  void OnOperationComplete(FileIOCallback* callback, int result);

  bool init_;
  bool mixed_;
  base::PlatformFile platform_file_;  
  base::PlatformFile sync_platform_file_;  

  DISALLOW_COPY_AND_ASSIGN(File);
};

}  

#endif  

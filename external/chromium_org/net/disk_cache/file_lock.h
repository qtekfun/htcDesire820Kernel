// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_DISK_CACHE_FILE_LOCK_H_
#define NET_DISK_CACHE_FILE_LOCK_H_

#include "net/base/net_export.h"
#include "net/disk_cache/disk_format_base.h"

namespace disk_cache {

class NET_EXPORT_PRIVATE FileLock {
 public:
  explicit FileLock(BlockFileHeader* header);
  virtual ~FileLock();

  
  virtual void Lock();
  virtual void Unlock();
 private:
  bool acquired_;
  volatile int32* updating_;
};

}  

#endif  

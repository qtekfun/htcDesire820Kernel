// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_DISK_CACHE_FILE_LOCK_H__
#define NET_DISK_CACHE_FILE_LOCK_H__
#pragma once

#include "net/disk_cache/disk_format.h"

namespace disk_cache {

class FileLock {
 public:
  explicit FileLock(BlockFileHeader* header);
  virtual ~FileLock() {
    Unlock();
  }
  
  virtual void Lock();
  virtual void Unlock();
 private:
  bool acquired_;
  volatile int32* updating_;
};

}  

#endif  

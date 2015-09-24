// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_SIMPLE_SIMPLE_VERSION_UPGRADE_H_
#define NET_DISK_CACHE_SIMPLE_SIMPLE_VERSION_UPGRADE_H_


#include "base/basictypes.h"
#include "net/base/net_export.h"

namespace base {
class FilePath;
}

namespace disk_cache {

NET_EXPORT_PRIVATE bool UpgradeSimpleCacheOnDisk(const base::FilePath& path);

struct NET_EXPORT_PRIVATE FakeIndexData {
  FakeIndexData();

  
  uint64 initial_magic_number;

  
  uint32 version;

  uint32 unused_must_be_zero1;
  uint32 unused_must_be_zero2;
};

NET_EXPORT_PRIVATE bool UpgradeIndexV5V6(const base::FilePath& cache_directory);

}  

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_CACHE_UTIL_H_
#define NET_DISK_CACHE_CACHE_UTIL_H_

#include "base/basictypes.h"
#include "net/base/net_export.h"
#include "net/disk_cache/disk_cache.h"

namespace base {
class FilePath;
}

namespace disk_cache {

NET_EXPORT_PRIVATE bool MoveCache(const base::FilePath& from_path,
                                  const base::FilePath& to_path);

NET_EXPORT_PRIVATE void DeleteCache(const base::FilePath& path,
                                    bool remove_folder);

NET_EXPORT_PRIVATE bool DeleteCacheFile(const base::FilePath& name);

bool DelayedCacheCleanup(const base::FilePath& full_path);

NET_EXPORT_PRIVATE int PreferredCacheSize(int64 available);

NET_EXPORT_PRIVATE extern const int kDefaultCacheSize;

}  

#endif  

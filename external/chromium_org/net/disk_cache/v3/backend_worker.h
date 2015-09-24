// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_DISK_CACHE_BACKEND_IMPL_H_
#define NET_DISK_CACHE_BACKEND_IMPL_H_

#include "base/containers/hash_tables.h"
#include "base/files/file_path.h"
#include "base/timer/timer.h"
#include "net/disk_cache/block_files.h"
#include "net/disk_cache/disk_cache.h"
#include "net/disk_cache/eviction.h"
#include "net/disk_cache/in_flight_backend_io.h"
#include "net/disk_cache/rankings.h"
#include "net/disk_cache/stats.h"
#include "net/disk_cache/stress_support.h"
#include "net/disk_cache/trace.h"

namespace disk_cache {

class NET_EXPORT_PRIVATE BackendImpl : public Backend {
  friend class Eviction;
 public:
  BackendImpl(const base::FilePath& path, base::MessageLoopProxy* cache_thread,
              net::NetLog* net_log);

  
  int Init(const CompletionCallback& callback);

 private:
  void CleanupCache();

  
  base::FilePath GetFileName(Addr address) const;

  
  bool CreateBackingStore(disk_cache::File* file);
  bool InitBackingStore(bool* file_created);

  
  void ReportError(int error);

  
  bool CheckIndex();

  base::FilePath path_;  
  BlockFiles block_files_;  
  bool init_;  

  DISALLOW_COPY_AND_ASSIGN(BackendImpl);
};

}  

#endif  

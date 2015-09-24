// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_SYNC_POINT_MANAGER_H_
#define CONTENT_COMMON_GPU_SYNC_POINT_MANAGER_H_

#include <vector>

#include "base/callback.h"
#include "base/containers/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_checker.h"

namespace content {

class SyncPointManager : public base::RefCountedThreadSafe<SyncPointManager> {
 public:
  SyncPointManager();

  
  
  uint32 GenerateSyncPoint();

  
  
  void RetireSyncPoint(uint32 sync_point);

  
  
  
  
  void AddSyncPointCallback(uint32 sync_point, const base::Closure& callback);

 private:
  friend class base::RefCountedThreadSafe<SyncPointManager>;
  typedef std::vector<base::Closure> ClosureList;
  typedef base::hash_map<uint32, ClosureList > SyncPointMap;

  ~SyncPointManager();

  base::ThreadChecker thread_checker_;

  
  
  base::Lock lock_;
  SyncPointMap sync_point_map_;
  uint32 next_sync_point_;

  DISALLOW_COPY_AND_ASSIGN(SyncPointManager);
};

}  

#endif  

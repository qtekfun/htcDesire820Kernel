// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_EVICTION_H_
#define NET_DISK_CACHE_EVICTION_H_
#pragma once

#include "base/basictypes.h"
#include "base/task.h"
#include "net/disk_cache/disk_format.h"
#include "net/disk_cache/rankings.h"

namespace disk_cache {

class BackendImpl;
class EntryImpl;

class Eviction {
 public:
  Eviction();
  ~Eviction();

  void Init(BackendImpl* backend);
  void Stop();

  
  
  
  void TrimCache(bool empty);

  
  void UpdateRank(EntryImpl* entry, bool modified);

  
  void OnOpenEntry(EntryImpl* entry);
  void OnCreateEntry(EntryImpl* entry);
  void OnDoomEntry(EntryImpl* entry);
  void OnDestroyEntry(EntryImpl* entry);

  
  void SetTestMode();
  void TrimDeletedList(bool empty);

 private:
  void PostDelayedTrim();
  void DelayedTrim();
  bool ShouldTrim();
  void ReportTrimTimes(EntryImpl* entry);
  Rankings::List GetListForEntry(EntryImpl* entry);
  bool EvictEntry(CacheRankingsBlock* node, bool empty, Rankings::List list);

  
  
  
  void TrimCacheV2(bool empty);
  void UpdateRankV2(EntryImpl* entry, bool modified);
  void OnOpenEntryV2(EntryImpl* entry);
  void OnCreateEntryV2(EntryImpl* entry);
  void OnDoomEntryV2(EntryImpl* entry);
  void OnDestroyEntryV2(EntryImpl* entry);
  Rankings::List GetListForEntryV2(EntryImpl* entry);
  void TrimDeleted(bool empty);
  bool RemoveDeletedNode(CacheRankingsBlock* node);

  bool NodeIsOldEnough(CacheRankingsBlock* node, int list);
  int SelectListByLength(Rankings::ScopedRankingsBlock* next);
  void ReportListStats();

  BackendImpl* backend_;
  Rankings* rankings_;
  IndexHeader* header_;
  int max_size_;
  int trim_delays_;
  bool new_eviction_;
  bool first_trim_;
  bool trimming_;
  bool delay_trim_;
  bool init_;
  bool test_mode_;
  bool in_experiment_;
  ScopedRunnableMethodFactory<Eviction> factory_;

  DISALLOW_COPY_AND_ASSIGN(Eviction);
};

}  

#endif  

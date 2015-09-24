// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_STATS_H_
#define NET_DISK_CACHE_STATS_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "net/disk_cache/addr.h"
#include "net/disk_cache/stats_histogram.h"

namespace base {
class HistogramSamples;
}  

namespace disk_cache {

typedef std::vector<std::pair<std::string, std::string> > StatsItems;

class Stats {
 public:
  static const int kDataSizesLength = 28;
  enum Counters {
    MIN_COUNTER = 0,
    OPEN_MISS = MIN_COUNTER,
    OPEN_HIT,
    CREATE_MISS,
    CREATE_HIT,
    RESURRECT_HIT,
    CREATE_ERROR,
    TRIM_ENTRY,
    DOOM_ENTRY,
    DOOM_CACHE,
    INVALID_ENTRY,
    OPEN_ENTRIES,  
    MAX_ENTRIES,  
    TIMER,
    READ_DATA,
    WRITE_DATA,
    OPEN_RANKINGS,  
    GET_RANKINGS,  
    FATAL_ERROR,
    LAST_REPORT,  
    LAST_REPORT_TIMER,  
    DOOM_RECENT,  
    UNUSED,  
    MAX_COUNTER
  };

  Stats();
  ~Stats();

  
  bool Init(void* data, int num_bytes, Addr address);

  
  void InitSizeHistogram();

  
  int StorageSize();

  
  void ModifyStorageStats(int32 old_size, int32 new_size);

  
  void OnEvent(Counters an_event);
  void SetCounter(Counters counter, int64 value);
  int64 GetCounter(Counters counter) const;

  void GetItems(StatsItems* items);
  int GetHitRatio() const;
  int GetResurrectRatio() const;
  void ResetRatios();

  
  int GetLargeEntriesSize();

  
  
  int SerializeStats(void* data, int num_bytes, Addr* address);

  
  
  int GetBucketRange(size_t i) const;
  void Snapshot(base::HistogramSamples* samples) const;

 private:
  int GetStatsBucket(int32 size);
  int GetRatio(Counters hit, Counters miss) const;

  Addr storage_addr_;
  int data_sizes_[kDataSizesLength];
  int64 counters_[MAX_COUNTER];
  StatsHistogram* size_histogram_;

  DISALLOW_COPY_AND_ASSIGN(Stats);
};

}  

#endif  

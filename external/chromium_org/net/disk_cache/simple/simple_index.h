// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_SIMPLE_SIMPLE_INDEX_H_
#define NET_DISK_CACHE_SIMPLE_SIMPLE_INDEX_H_

#include <list>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/containers/hash_tables.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/single_thread_task_runner.h"
#include "base/threading/thread_checker.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "net/base/cache_type.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"

#if defined(OS_ANDROID)
#include "base/android/activity_status.h"
#endif

class Pickle;
class PickleIterator;

namespace disk_cache {

class SimpleIndexDelegate;
class SimpleIndexFile;
struct SimpleIndexLoadResult;

class NET_EXPORT_PRIVATE EntryMetadata {
 public:
  EntryMetadata();
  EntryMetadata(base::Time last_used_time, int entry_size);

  base::Time GetLastUsedTime() const;
  void SetLastUsedTime(const base::Time& last_used_time);

  int GetEntrySize() const { return entry_size_; }
  void SetEntrySize(int entry_size) { entry_size_ = entry_size; }

  
  void Serialize(Pickle* pickle) const;
  bool Deserialize(PickleIterator* it);

  static base::TimeDelta GetLowerEpsilonForTimeComparisons() {
    return base::TimeDelta::FromSeconds(1);
  }
  static base::TimeDelta GetUpperEpsilonForTimeComparisons() {
    return base::TimeDelta();
  }

 private:
  friend class SimpleIndexFileTest;

  
  

  uint32 last_used_time_seconds_since_epoch_;

  int32 entry_size_;  
};
COMPILE_ASSERT(sizeof(EntryMetadata) == 8, metadata_size);

class NET_EXPORT_PRIVATE SimpleIndex
    : public base::SupportsWeakPtr<SimpleIndex> {
 public:
  typedef std::vector<uint64> HashList;

  SimpleIndex(base::SingleThreadTaskRunner* io_thread,
              SimpleIndexDelegate* delegate,
              net::CacheType cache_type,
              scoped_ptr<SimpleIndexFile> simple_index_file);

  virtual ~SimpleIndex();

  void Initialize(base::Time cache_mtime);

  bool SetMaxSize(int max_bytes);
  int max_size() const { return max_size_; }

  void Insert(uint64 entry_hash);
  void Remove(uint64 entry_hash);

  
  bool Has(uint64 entry_hash) const;

  
  
  bool UseIfExists(uint64 entry_hash);

  void WriteToDisk();

  
  
  
  bool UpdateEntrySize(uint64 entry_hash, int entry_size);

  typedef base::hash_map<uint64, EntryMetadata> EntrySet;

  static void InsertInEntrySet(uint64 entry_hash,
                               const EntryMetadata& entry_metadata,
                               EntrySet* entry_set);

  
  int ExecuteWhenReady(const net::CompletionCallback& callback);

  
  
  
  
  scoped_ptr<HashList> GetEntriesBetween(const base::Time initial_time,
                                         const base::Time end_time);

  
  scoped_ptr<HashList> GetAllHashes();

  
  int32 GetEntryCount() const;

  
  bool initialized() const { return initialized_; }

 private:
  friend class SimpleIndexTest;
  FRIEND_TEST_ALL_PREFIXES(SimpleIndexTest, IndexSizeCorrectOnMerge);
  FRIEND_TEST_ALL_PREFIXES(SimpleIndexTest, DiskWriteQueued);
  FRIEND_TEST_ALL_PREFIXES(SimpleIndexTest, DiskWriteExecuted);
  FRIEND_TEST_ALL_PREFIXES(SimpleIndexTest, DiskWritePostponed);

  void StartEvictionIfNeeded();
  void EvictionDone(int result);

  void PostponeWritingToDisk();

  void UpdateEntryIteratorSize(EntrySet::iterator* it, int entry_size);

  
  void MergeInitializingSet(scoped_ptr<SimpleIndexLoadResult> load_result);

#if defined(OS_ANDROID)
  void OnActivityStateChange(base::android::ActivityState state);

  scoped_ptr<base::android::ActivityStatus::Listener> activity_status_listener_;
#endif

  
  SimpleIndexDelegate* delegate_;

  EntrySet entries_set_;

  const net::CacheType cache_type_;
  uint64 cache_size_;  
  uint64 max_size_;
  uint64 high_watermark_;
  uint64 low_watermark_;
  bool eviction_in_progress_;
  base::TimeTicks eviction_start_time_;

  
  
  base::hash_set<uint64> removed_entries_;
  bool initialized_;

  scoped_ptr<SimpleIndexFile> index_file_;

  scoped_refptr<base::SingleThreadTaskRunner> io_thread_;

  
  
  base::ThreadChecker io_thread_checker_;

  
  
  
  base::TimeTicks last_write_to_disk_;

  base::OneShotTimer<SimpleIndex> write_to_disk_timer_;
  base::Closure write_to_disk_cb_;

  typedef std::list<net::CompletionCallback> CallbackList;
  CallbackList to_run_when_initialized_;

  
  
  
  bool app_on_background_;

  
  
  
  int foreground_flush_delay_;
  int background_flush_delay_;
};

}  

#endif  

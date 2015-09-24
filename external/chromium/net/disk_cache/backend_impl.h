// Copyright (c) 2006-2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_DISK_CACHE_BACKEND_IMPL_H_
#define NET_DISK_CACHE_BACKEND_IMPL_H_
#pragma once

#include "base/file_path.h"
#include "base/hash_tables.h"
#include "base/timer.h"
#include "net/disk_cache/block_files.h"
#include "net/disk_cache/disk_cache.h"
#include "net/disk_cache/eviction.h"
#include "net/disk_cache/in_flight_backend_io.h"
#include "net/disk_cache/rankings.h"
#include "net/disk_cache/stats.h"
#include "net/disk_cache/trace.h"

namespace net {
class NetLog;
}  

namespace disk_cache {

enum BackendFlags {
  kNone = 0,
  kMask = 1,                    
  kMaxSize = 1 << 1,            
  kUnitTestMode = 1 << 2,       
  kUpgradeMode = 1 << 3,        
  kNewEviction = 1 << 4,        
  kNoRandom = 1 << 5,           
  kNoLoadProtection = 1 << 6,   
  kNoBuffering = 1 << 7         
};

class BackendImpl : public Backend {
  friend class Eviction;
 public:
  BackendImpl(const FilePath& path, base::MessageLoopProxy* cache_thread,
              net::NetLog* net_log);
  
  BackendImpl(const FilePath& path, uint32 mask,
              base::MessageLoopProxy* cache_thread, net::NetLog* net_log);
  ~BackendImpl();

  
  
  static int CreateBackend(const FilePath& full_path, bool force,
                           int max_bytes, net::CacheType type,
                           uint32 flags, base::MessageLoopProxy* thread,
                           net::NetLog* net_log, Backend** backend,
                           CompletionCallback* callback);

  
  int Init(CompletionCallback* callback);

  
  int SyncInit();
  void CleanupCache();

  
  int OpenPrevEntry(void** iter, Entry** prev_entry,
                    CompletionCallback* callback);

  
  int SyncOpenEntry(const std::string& key, Entry** entry);
  int SyncCreateEntry(const std::string& key, Entry** entry);
  int SyncDoomEntry(const std::string& key);
  int SyncDoomAllEntries();
  int SyncDoomEntriesBetween(const base::Time initial_time,
                             const base::Time end_time);
  int SyncDoomEntriesSince(const base::Time initial_time);
  int SyncOpenNextEntry(void** iter, Entry** next_entry);
  int SyncOpenPrevEntry(void** iter, Entry** prev_entry);
  void SyncEndEnumeration(void* iter);

  
  EntryImpl* OpenEntryImpl(const std::string& key);
  EntryImpl* CreateEntryImpl(const std::string& key);
  EntryImpl* OpenNextEntryImpl(void** iter);
  EntryImpl* OpenPrevEntryImpl(void** iter);

  
  bool SetMaxSize(int max_bytes);

  
  void SetType(net::CacheType type);

  
  FilePath GetFileName(Addr address) const;

  
  MappedFile* File(Addr address);

  InFlightBackendIO* background_queue() {
    return &background_queue_;
  }

  
  bool CreateExternalFile(Addr* address);

  
  bool CreateBlock(FileType block_type, int block_count,
                   Addr* block_address);

  
  
  void DeleteBlock(Addr block_address, bool deep);

  
  LruData* GetLruData();

  
  void UpdateRank(EntryImpl* entry, bool modified);

  
  
  void RecoveredEntry(CacheRankingsBlock* rankings);

  
  void InternalDoomEntry(EntryImpl* entry);

  
  void RemoveEntry(EntryImpl* entry);

  
  
  
  void OnEntryDestroyBegin(Addr address);

  
  
  void OnEntryDestroyEnd();

  
  
  
  EntryImpl* GetOpenEntry(CacheRankingsBlock* rankings) const;

  
  int32 GetCurrentEntryId() const;

  
  int MaxFileSize() const;

  
  void ModifyStorageSize(int32 old_size, int32 new_size);

  
  void TooMuchStorageRequested(int32 size);

  
  bool IsAllocAllowed(int current_size, int new_size);

  
  void BufferDeleted(int size);

  
  int GetTotalBuffersSize() const {
    return buffer_bytes_;
  }

  
  bool IsLoaded() const;

  
  
  
  std::string HistogramName(const char* name, int experiment) const;

  net::CacheType cache_type() const {
    return cache_type_;
  }

  
  base::WeakPtr<BackendImpl> GetWeakPtr();

  
  int GetSizeGroup() const;

  
  
  
  bool ShouldReportAgain();

  
  void FirstEviction();

  
  void CriticalError(int error);

  
  void ReportError(int error);

  
  void OnEvent(Stats::Counters an_event);

  
  void OnRead(int bytes);
  void OnWrite(int bytes);

  
  void OnStatsTimer();

  
  void IncrementIoCount();
  void DecrementIoCount();

  
  void SetUnitTestMode();

  
  void SetUpgradeMode();

  
  void SetNewEviction();

  
  void SetFlags(uint32 flags);

  
  void ClearRefCountForTest();

  
  int FlushQueueForTest(CompletionCallback* callback);

  
  
  int RunTaskForTest(Task* task, CompletionCallback* callback);

  
  
  void TrimForTest(bool empty);

  
  
  void TrimDeletedListForTest(bool empty);

  
  
  int SelfCheck();

  
  virtual int32 GetEntryCount() const;
  virtual int OpenEntry(const std::string& key, Entry** entry,
                        CompletionCallback* callback);
  virtual int CreateEntry(const std::string& key, Entry** entry,
                          CompletionCallback* callback);
  virtual int DoomEntry(const std::string& key, CompletionCallback* callback);
  virtual int DoomAllEntries(CompletionCallback* callback);
  virtual int DoomEntriesBetween(const base::Time initial_time,
                                 const base::Time end_time,
                                 CompletionCallback* callback);
  virtual int DoomEntriesSince(const base::Time initial_time,
                               CompletionCallback* callback);
  virtual int OpenNextEntry(void** iter, Entry** next_entry,
                            CompletionCallback* callback);
  virtual void EndEnumeration(void** iter);
  virtual void GetStats(StatsItems* stats);

 private:
  typedef base::hash_map<CacheAddr, EntryImpl*> EntriesMap;

  
  bool CreateBackingStore(disk_cache::File* file);
  bool InitBackingStore(bool* file_created);
  void AdjustMaxCacheSize(int table_len);

  
  void RestartCache(bool failure);
  void PrepareForRestart();

  
  
  int NewEntry(Addr address, EntryImpl** entry);

  
  
  
  
  
  
  
  EntryImpl* MatchEntry(const std::string& key, uint32 hash, bool find_parent,
                        Addr entry_addr, bool* match_error);

  
  EntryImpl* OpenFollowingEntry(bool forward, void** iter);

  
  
  
  bool OpenFollowingEntryFromList(bool forward, Rankings::List list,
                                  CacheRankingsBlock** from_entry,
                                  EntryImpl** next_entry);

  
  EntryImpl* GetEnumeratedEntry(CacheRankingsBlock* next, Rankings::List list);

  
  EntryImpl* ResurrectEntry(EntryImpl* deleted_entry);

  void DestroyInvalidEntry(EntryImpl* entry);

  
  void AddStorageSize(int32 bytes);
  void SubstractStorageSize(int32 bytes);

  
  void IncreaseNumRefs();
  void DecreaseNumRefs();
  void IncreaseNumEntries();
  void DecreaseNumEntries();

  
  void LogStats();

  
  void ReportStats();

  
  void UpgradeTo2_1();

  
  bool CheckIndex();

  
  int CheckAllEntries();

  
  bool CheckEntry(EntryImpl* cache_entry);

  
  int MaxBuffersSize();

  InFlightBackendIO background_queue_;  
  scoped_refptr<MappedFile> index_;  
  FilePath path_;  
  Index* data_;  
  BlockFiles block_files_;  
  Rankings rankings_;  
  uint32 mask_;  
  int32 max_size_;  
  Eviction eviction_;  
  EntriesMap open_entries_;  
  int num_refs_;  
  int max_refs_;  
  int num_pending_io_;  
  int entry_count_;  
  int byte_count_;  // Number of bytes read/written lately.
  int buffer_bytes_;  
  int io_delay_;  
  net::CacheType cache_type_;
  int uma_report_;  
  uint32 user_flags_;  
  bool init_;  
  bool restarted_;
  bool unit_test_;
  bool read_only_;  
  bool disabled_;
  bool new_eviction_;  
  bool first_timer_;  
  bool throttle_requests_;

  net::NetLog* net_log_;

  Stats stats_;  
  base::RepeatingTimer<BackendImpl> timer_;  
  base::WaitableEvent done_;  
  scoped_refptr<TraceObject> trace_object_;  
  ScopedRunnableMethodFactory<BackendImpl> factory_;
  base::WeakPtrFactory<BackendImpl> ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(BackendImpl);
};

int PreferedCacheSize(int64 available);

}  

#endif  

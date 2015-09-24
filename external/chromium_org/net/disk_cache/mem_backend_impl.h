// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_DISK_CACHE_MEM_BACKEND_IMPL_H__
#define NET_DISK_CACHE_MEM_BACKEND_IMPL_H__

#include "base/compiler_specific.h"
#include "base/containers/hash_tables.h"
#include "net/disk_cache/disk_cache.h"
#include "net/disk_cache/mem_rankings.h"

namespace net {
class NetLog;
}  

namespace disk_cache {

class MemEntryImpl;

class NET_EXPORT_PRIVATE MemBackendImpl : public Backend {
 public:
  explicit MemBackendImpl(net::NetLog* net_log);
  virtual ~MemBackendImpl();

  
  
  
  
  
  static scoped_ptr<Backend> CreateBackend(int max_bytes, net::NetLog* net_log);

  
  bool Init();

  
  bool SetMaxSize(int max_bytes);

  
  void InternalDoomEntry(MemEntryImpl* entry);

  
  void UpdateRank(MemEntryImpl* node);

  
  void ModifyStorageSize(int32 old_size, int32 new_size);

  
  int MaxFileSize() const;

  
  
  
  void InsertIntoRankingList(MemEntryImpl* entry);

  
  
  void RemoveFromRankingList(MemEntryImpl* entry);

  
  virtual net::CacheType GetCacheType() const OVERRIDE;
  virtual int32 GetEntryCount() const OVERRIDE;
  virtual int OpenEntry(const std::string& key, Entry** entry,
                        const CompletionCallback& callback) OVERRIDE;
  virtual int CreateEntry(const std::string& key, Entry** entry,
                          const CompletionCallback& callback) OVERRIDE;
  virtual int DoomEntry(const std::string& key,
                        const CompletionCallback& callback) OVERRIDE;
  virtual int DoomAllEntries(const CompletionCallback& callback) OVERRIDE;
  virtual int DoomEntriesBetween(base::Time initial_time,
                                 base::Time end_time,
                                 const CompletionCallback& callback) OVERRIDE;
  virtual int DoomEntriesSince(base::Time initial_time,
                               const CompletionCallback& callback) OVERRIDE;
  virtual int OpenNextEntry(void** iter, Entry** next_entry,
                            const CompletionCallback& callback) OVERRIDE;
  virtual void EndEnumeration(void** iter) OVERRIDE;
  virtual void GetStats(
      std::vector<std::pair<std::string, std::string> >* stats) OVERRIDE {}
  virtual void OnExternalCacheHit(const std::string& key) OVERRIDE;

 private:
  typedef base::hash_map<std::string, MemEntryImpl*> EntryMap;

  
  bool OpenEntry(const std::string& key, Entry** entry);
  bool CreateEntry(const std::string& key, Entry** entry);
  bool DoomEntry(const std::string& key);
  bool DoomAllEntries();
  bool DoomEntriesBetween(const base::Time initial_time,
                          const base::Time end_time);
  bool DoomEntriesSince(const base::Time initial_time);
  bool OpenNextEntry(void** iter, Entry** next_entry);

  
  
  
  void TrimCache(bool empty);

  
  void AddStorageSize(int32 bytes);
  void SubstractStorageSize(int32 bytes);

  EntryMap entries_;
  MemRankings rankings_;  
  int32 max_size_;        
  int32 current_size_;

  net::NetLog* net_log_;

  DISALLOW_COPY_AND_ASSIGN(MemBackendImpl);
};

}  

#endif  

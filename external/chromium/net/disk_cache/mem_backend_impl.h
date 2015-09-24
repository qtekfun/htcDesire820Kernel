// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_DISK_CACHE_MEM_BACKEND_IMPL_H__
#define NET_DISK_CACHE_MEM_BACKEND_IMPL_H__
#pragma once

#include "base/hash_tables.h"

#include "net/disk_cache/disk_cache.h"
#include "net/disk_cache/mem_rankings.h"

namespace net {
class NetLog;
}  

namespace disk_cache {

class MemEntryImpl;

class MemBackendImpl : public Backend {
 public:
  explicit MemBackendImpl(net::NetLog* net_log);
  ~MemBackendImpl();

  
  
  
  
  
  static Backend* CreateBackend(int max_bytes, net::NetLog* net_log);

  
  bool Init();

  
  bool SetMaxSize(int max_bytes);

  
  void InternalDoomEntry(MemEntryImpl* entry);

  
  void UpdateRank(MemEntryImpl* node);

  
  void ModifyStorageSize(int32 old_size, int32 new_size);

  
  int MaxFileSize() const;

  
  
  
  void InsertIntoRankingList(MemEntryImpl* entry);

  
  
  void RemoveFromRankingList(MemEntryImpl* entry);

  
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
  virtual void GetStats(
      std::vector<std::pair<std::string, std::string> >* stats) {}

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

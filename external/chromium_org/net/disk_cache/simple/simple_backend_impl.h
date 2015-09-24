// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_SIMPLE_SIMPLE_BACKEND_IMPL_H_
#define NET_DISK_CACHE_SIMPLE_SIMPLE_BACKEND_IMPL_H_

#include <string>
#include <utility>
#include <vector>

#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/containers/hash_tables.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/task_runner.h"
#include "base/time/time.h"
#include "net/base/cache_type.h"
#include "net/disk_cache/disk_cache.h"
#include "net/disk_cache/simple/simple_entry_impl.h"
#include "net/disk_cache/simple/simple_index_delegate.h"

namespace base {
class SingleThreadTaskRunner;
class TaskRunner;
}

namespace disk_cache {


class SimpleEntryImpl;
class SimpleIndex;

class NET_EXPORT_PRIVATE SimpleBackendImpl : public Backend,
    public SimpleIndexDelegate,
    public base::SupportsWeakPtr<SimpleBackendImpl> {
 public:
  SimpleBackendImpl(const base::FilePath& path, int max_bytes,
                    net::CacheType cache_type,
                    base::SingleThreadTaskRunner* cache_thread,
                    net::NetLog* net_log);

  virtual ~SimpleBackendImpl();

  net::CacheType cache_type() const { return cache_type_; }
  SimpleIndex* index() { return index_.get(); }

  base::TaskRunner* worker_pool() { return worker_pool_.get(); }

  int Init(const CompletionCallback& completion_callback);

  
  bool SetMaxSize(int max_bytes);

  
  int GetMaxFileSize() const;

  
  
  void OnDeactivated(const SimpleEntryImpl* entry);

  
  static void FlushWorkerPoolForTesting();

  
  
  void OnDoomStart(uint64 entry_hash);

  
  
  
  void OnDoomComplete(uint64 entry_hash);

  
  virtual void DoomEntries(std::vector<uint64>* entry_hashes,
                           const CompletionCallback& callback) OVERRIDE;

  
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
      std::vector<std::pair<std::string, std::string> >* stats) OVERRIDE;
  virtual void OnExternalCacheHit(const std::string& key) OVERRIDE;

 private:
  typedef base::hash_map<uint64, base::WeakPtr<SimpleEntryImpl> > EntryMap;

  typedef base::Callback<void(base::Time mtime, uint64 max_size, int result)>
      InitializeIndexCallback;

  
  struct DiskStatResult {
    base::Time cache_dir_mtime;
    uint64 max_size;
    bool detected_magic_number_mismatch;
    int net_error;
  };

  void InitializeIndex(const CompletionCallback& callback,
                       const DiskStatResult& result);

  
  
  void IndexReadyForDoom(base::Time initial_time,
                         base::Time end_time,
                         const CompletionCallback& callback,
                         int result);

  
  
  static DiskStatResult InitCacheStructureOnDisk(const base::FilePath& path,
                                                 uint64 suggested_max_size);

  
  
  scoped_refptr<SimpleEntryImpl> CreateOrFindActiveEntry(
      uint64 entry_hash,
      const std::string& key);

  
  
  
  
  int OpenEntryFromHash(uint64 entry_hash,
                        Entry** entry,
                        const CompletionCallback& callback);

  
  
  
  int DoomEntryFromHash(uint64 entry_hash, const CompletionCallback & callback);

  
  
  
  void GetNextEntryInIterator(void** iter,
                              Entry** next_entry,
                              const CompletionCallback& callback,
                              int error_code);

  
  
  
  
  void OnEntryOpenedFromHash(uint64 hash,
                             Entry** entry,
                             scoped_refptr<SimpleEntryImpl> simple_entry,
                             const CompletionCallback& callback,
                             int error_code);

  
  
  void OnEntryOpenedFromKey(const std::string key,
                            Entry** entry,
                            scoped_refptr<SimpleEntryImpl> simple_entry,
                            const CompletionCallback& callback,
                            int error_code);

  
  
  
  void CheckIterationReturnValue(void** iter,
                                 Entry** entry,
                                 const CompletionCallback& callback,
                                 int error_code);

  
  
  void DoomEntriesComplete(scoped_ptr<std::vector<uint64> > entry_hashes,
                           const CompletionCallback& callback,
                           int result);

  const base::FilePath path_;
  const net::CacheType cache_type_;
  scoped_ptr<SimpleIndex> index_;
  const scoped_refptr<base::SingleThreadTaskRunner> cache_thread_;
  scoped_refptr<base::TaskRunner> worker_pool_;

  int orig_max_size_;
  const SimpleEntryImpl::OperationsMode entry_operations_mode_;

  EntryMap active_entries_;

  
  
  
  
  base::hash_map<uint64, std::vector<base::Closure> > entries_pending_doom_;

  net::NetLog* const net_log_;
};

}  

#endif  

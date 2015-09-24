// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_MEM_ENTRY_IMPL_H_
#define NET_DISK_CACHE_MEM_ENTRY_IMPL_H_

#include "base/containers/hash_tables.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/net_log.h"
#include "net/disk_cache/disk_cache.h"

namespace disk_cache {

class MemBackendImpl;


class MemEntryImpl : public Entry {
 public:
  enum EntryType {
    kParentEntry,
    kChildEntry,
  };

  explicit MemEntryImpl(MemBackendImpl* backend);

  
  
  bool CreateEntry(const std::string& key, net::NetLog* net_log);

  
  void InternalDoom();

  void Open();
  bool InUse();

  MemEntryImpl* next() const {
    return next_;
  }

  MemEntryImpl* prev() const {
    return prev_;
  }

  void set_next(MemEntryImpl* next) {
    next_ = next;
  }

  void set_prev(MemEntryImpl* prev) {
    prev_ = prev;
  }

  EntryType type() const {
    return parent_ ? kChildEntry : kParentEntry;
  }

  const net::BoundNetLog& net_log() {
    return net_log_;
  }

  
  virtual void Doom() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual std::string GetKey() const OVERRIDE;
  virtual base::Time GetLastUsed() const OVERRIDE;
  virtual base::Time GetLastModified() const OVERRIDE;
  virtual int32 GetDataSize(int index) const OVERRIDE;
  virtual int ReadData(int index, int offset, IOBuffer* buf, int buf_len,
                       const CompletionCallback& callback) OVERRIDE;
  virtual int WriteData(int index, int offset, IOBuffer* buf, int buf_len,
                        const CompletionCallback& callback,
                        bool truncate) OVERRIDE;
  virtual int ReadSparseData(int64 offset, IOBuffer* buf, int buf_len,
                             const CompletionCallback& callback) OVERRIDE;
  virtual int WriteSparseData(int64 offset, IOBuffer* buf, int buf_len,
                              const CompletionCallback& callback) OVERRIDE;
  virtual int GetAvailableRange(int64 offset, int len, int64* start,
                                const CompletionCallback& callback) OVERRIDE;
  virtual bool CouldBeSparse() const OVERRIDE;
  virtual void CancelSparseIO() OVERRIDE {}
  virtual int ReadyForSparseIO(const CompletionCallback& callback) OVERRIDE;

 private:
  typedef base::hash_map<int, MemEntryImpl*> EntryMap;

  enum {
    NUM_STREAMS = 3
  };

  virtual ~MemEntryImpl();

  
  
  int InternalReadData(int index, int offset, IOBuffer* buf, int buf_len);
  int InternalWriteData(int index, int offset, IOBuffer* buf, int buf_len,
                        bool truncate);
  int InternalReadSparseData(int64 offset, IOBuffer* buf, int buf_len);
  int InternalWriteSparseData(int64 offset, IOBuffer* buf, int buf_len);

  
  int GetAvailableRange(int64 offset, int len, int64* start);

  
  void PrepareTarget(int index, int offset, int buf_len);

  
  void UpdateRank(bool modified);

  
  
  bool InitSparseInfo();

  
  
  
  bool InitChildEntry(MemEntryImpl* parent, int child_id, net::NetLog* net_log);

  
  
  
  
  MemEntryImpl* OpenChild(int64 offset, bool create);

  
  
  
  int FindNextChild(int64 offset, int len, MemEntryImpl** child);

  
  void DetachChild(int child_id);

  std::string key_;
  std::vector<char> data_[NUM_STREAMS];  
  int32 data_size_[NUM_STREAMS];
  int ref_count_;

  int child_id_;              
  int child_first_pos_;       
                              
  MemEntryImpl* next_;        
  MemEntryImpl* prev_;
  MemEntryImpl* parent_;      
  scoped_ptr<EntryMap> children_;

  base::Time last_modified_;  
  base::Time last_used_;
  MemBackendImpl* backend_;   
  bool doomed_;               

  net::BoundNetLog net_log_;

  DISALLOW_COPY_AND_ASSIGN(MemEntryImpl);
};

}  

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_ENTRY_IMPL_H_
#define NET_DISK_CACHE_ENTRY_IMPL_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "net/base/net_log.h"
#include "net/disk_cache/disk_cache.h"
#include "net/disk_cache/storage_block.h"
#include "net/disk_cache/storage_block-inl.h"

namespace disk_cache {

class BackendImpl;
class SparseControl;

class EntryImpl : public Entry, public base::RefCounted<EntryImpl> {
  friend class base::RefCounted<EntryImpl>;
  friend class SparseControl;
 public:
  enum Operation {
    kRead,
    kWrite,
    kSparseRead,
    kSparseWrite,
    kAsyncIO
  };

  EntryImpl(BackendImpl* backend, Addr address, bool read_only);

  
  void DoomImpl();
  int ReadDataImpl(int index, int offset, net::IOBuffer* buf, int buf_len,
                   CompletionCallback* callback);
  int WriteDataImpl(int index, int offset, net::IOBuffer* buf, int buf_len,
                    CompletionCallback* callback, bool truncate);
  int ReadSparseDataImpl(int64 offset, net::IOBuffer* buf, int buf_len,
                         CompletionCallback* callback);
  int WriteSparseDataImpl(int64 offset, net::IOBuffer* buf, int buf_len,
                          CompletionCallback* callback);
  int GetAvailableRangeImpl(int64 offset, int len, int64* start);
  void CancelSparseIOImpl();
  int ReadyForSparseIOImpl(CompletionCallback* callback);

  inline CacheEntryBlock* entry() {
    return &entry_;
  }

  inline CacheRankingsBlock* rankings() {
    return &node_;
  }

  uint32 GetHash();

  
  
  bool CreateEntry(Addr node_address, const std::string& key, uint32 hash);

  
  bool IsSameEntry(const std::string& key, uint32 hash);

  
  void InternalDoom();

  
  
  void DeleteEntryData(bool everything);

  
  
  CacheAddr GetNextAddress();

  
  
  void SetNextAddress(Addr address);

  
  bool LoadNodeAddress();

  
  
  
  bool Update();

  bool dirty() {
    return dirty_;
  }

  bool doomed() {
    return doomed_;
  }

  
  
  void SetDirtyFlag(int32 current_id);

  
  void SetPointerForInvalidEntry(int32 new_id);

  
  
  bool LeaveRankingsBehind();

  
  bool SanityCheck();
  bool DataSanityCheck();

  
  void FixForDelete();

  
  void IncrementIoCount();
  void DecrementIoCount();

  
  
  void SetTimes(base::Time last_used, base::Time last_modified);

  
  void ReportIOTime(Operation op, const base::TimeTicks& start);

  
  
  
  
  void BeginLogging(net::NetLog* net_log, bool created);

  const net::BoundNetLog& net_log() const;

  
  static int NumBlocksForEntry(int key_size);

  
  virtual void Doom();
  virtual void Close();
  virtual std::string GetKey() const;
  virtual base::Time GetLastUsed() const;
  virtual base::Time GetLastModified() const;
  virtual int32 GetDataSize(int index) const;
  virtual int ReadData(int index, int offset, net::IOBuffer* buf, int buf_len,
                       net::CompletionCallback* completion_callback);
  virtual int WriteData(int index, int offset, net::IOBuffer* buf, int buf_len,
                        net::CompletionCallback* completion_callback,
                        bool truncate);
  virtual int ReadSparseData(int64 offset, net::IOBuffer* buf, int buf_len,
                             net::CompletionCallback* completion_callback);
  virtual int WriteSparseData(int64 offset, net::IOBuffer* buf, int buf_len,
                              net::CompletionCallback* completion_callback);
  virtual int GetAvailableRange(int64 offset, int len, int64* start,
                                CompletionCallback* callback);
  virtual bool CouldBeSparse() const;
  virtual void CancelSparseIO();
  virtual int ReadyForSparseIO(net::CompletionCallback* completion_callback);

 private:
  enum {
     kNumStreams = 3
  };
  class UserBuffer;

  ~EntryImpl();

  
  
  int InternalReadData(int index, int offset, net::IOBuffer* buf,
                       int buf_len, CompletionCallback* callback);
  int InternalWriteData(int index, int offset, net::IOBuffer* buf, int buf_len,
                        CompletionCallback* callback, bool truncate);

  
  bool CreateDataBlock(int index, int size);

  
  bool CreateBlock(int size, Addr* address);

  
  
  
  
  void DeleteData(Addr address, int index);

  
  void UpdateRank(bool modified);

  
  File* GetBackingFile(Addr address, int index);

  
  File* GetExternalFile(Addr address, int index);

  
  
  bool PrepareTarget(int index, int offset, int buf_len, bool truncate);

  
  
  bool HandleTruncation(int index, int offset, int buf_len);

  
  bool CopyToLocalBuffer(int index);

  
  bool MoveToLocalBuffer(int index);

  
  bool ImportSeparateFile(int index, int new_size);

  
  
  bool PrepareBuffer(int index, int offset, int buf_len);

  
  
  bool Flush(int index, int min_len);

  
  void UpdateSize(int index, int old_size, int new_size);

  
  int InitSparseData();

  
  void SetEntryFlags(uint32 flags);

  
  uint32 GetEntryFlags();

  
  
  
  
  
  
  
  
  void GetData(int index, char** buffer, Addr* address);

  
  void Log(const char* msg);

  CacheEntryBlock entry_;     
  CacheRankingsBlock node_;   
  BackendImpl* backend_;      
  scoped_ptr<UserBuffer> user_buffers_[kNumStreams];  
  
  scoped_refptr<File> files_[kNumStreams + 1];
  mutable std::string key_;           
  int unreported_size_[kNumStreams];  
  bool doomed_;               
  bool read_only_;            
  bool dirty_;                
  scoped_ptr<SparseControl> sparse_;  

  net::BoundNetLog net_log_;

  DISALLOW_COPY_AND_ASSIGN(EntryImpl);
};

}  

#endif  

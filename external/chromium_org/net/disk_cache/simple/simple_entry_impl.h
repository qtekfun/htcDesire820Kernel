// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_SIMPLE_SIMPLE_ENTRY_IMPL_H_
#define NET_DISK_CACHE_SIMPLE_SIMPLE_ENTRY_IMPL_H_

#include <queue>
#include <string>

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/thread_checker.h"
#include "net/base/cache_type.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/disk_cache/disk_cache.h"
#include "net/disk_cache/simple/simple_entry_format.h"
#include "net/disk_cache/simple/simple_entry_operation.h"

namespace base {
class TaskRunner;
}

namespace net {
class GrowableIOBuffer;
class IOBuffer;
}

namespace disk_cache {

class SimpleBackendImpl;
class SimpleSynchronousEntry;
class SimpleEntryStat;
struct SimpleEntryCreationResults;

class NET_EXPORT_PRIVATE SimpleEntryImpl : public Entry,
    public base::RefCounted<SimpleEntryImpl>,
    public base::SupportsWeakPtr<SimpleEntryImpl> {
  friend class base::RefCounted<SimpleEntryImpl>;
 public:
  enum OperationsMode {
    NON_OPTIMISTIC_OPERATIONS,
    OPTIMISTIC_OPERATIONS,
  };

  SimpleEntryImpl(net::CacheType cache_type,
                  const base::FilePath& path,
                  uint64 entry_hash,
                  OperationsMode operations_mode,
                  SimpleBackendImpl* backend,
                  net::NetLog* net_log);

  
  
  int OpenEntry(Entry** entry, const CompletionCallback& callback);

  
  int CreateEntry(Entry** entry, const CompletionCallback& callback);

  
  int DoomEntry(const CompletionCallback& callback);

  const std::string& key() const { return key_; }
  uint64 entry_hash() const { return entry_hash_; }
  void SetKey(const std::string& key);

  
  virtual void Doom() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual std::string GetKey() const OVERRIDE;
  virtual base::Time GetLastUsed() const OVERRIDE;
  virtual base::Time GetLastModified() const OVERRIDE;
  virtual int32 GetDataSize(int index) const OVERRIDE;
  virtual int ReadData(int stream_index,
                       int offset,
                       net::IOBuffer* buf,
                       int buf_len,
                       const CompletionCallback& callback) OVERRIDE;
  virtual int WriteData(int stream_index,
                        int offset,
                        net::IOBuffer* buf,
                        int buf_len,
                        const CompletionCallback& callback,
                        bool truncate) OVERRIDE;
  virtual int ReadSparseData(int64 offset,
                             net::IOBuffer* buf,
                             int buf_len,
                             const CompletionCallback& callback) OVERRIDE;
  virtual int WriteSparseData(int64 offset,
                              net::IOBuffer* buf,
                              int buf_len,
                              const CompletionCallback& callback) OVERRIDE;
  virtual int GetAvailableRange(int64 offset,
                                int len,
                                int64* start,
                                const CompletionCallback& callback) OVERRIDE;
  virtual bool CouldBeSparse() const OVERRIDE;
  virtual void CancelSparseIO() OVERRIDE;
  virtual int ReadyForSparseIO(const CompletionCallback& callback) OVERRIDE;

 private:
  class ScopedOperationRunner;
  friend class ScopedOperationRunner;

  enum State {
    
    
    
    STATE_UNINITIALIZED,

    
    STATE_READY,

    
    
    STATE_IO_PENDING,

    
    
    STATE_FAILURE,
  };

  
  enum CheckCrcResult {
    CRC_CHECK_NEVER_READ_TO_END = 0,
    CRC_CHECK_NOT_DONE = 1,
    CRC_CHECK_DONE = 2,
    CRC_CHECK_NEVER_READ_AT_ALL = 3,
    CRC_CHECK_MAX = 4,
  };

  virtual ~SimpleEntryImpl();

  
  
  
  
  void PostClientCallback(const CompletionCallback& callback, int result);

  
  void MakeUninitialized();

  
  
  void ReturnEntryToCaller(Entry** out_entry);

  
  
  void RemoveSelfFromBackend();

  
  
  
  void MarkAsDoomed();

  
  
  
  
  void RunNextOperationIfNeeded();

  void OpenEntryInternal(bool have_index,
                         const CompletionCallback& callback,
                         Entry** out_entry);

  void CreateEntryInternal(bool have_index,
                           const CompletionCallback& callback,
                           Entry** out_entry);

  void CloseInternal();

  void ReadDataInternal(int index,
                        int offset,
                        net::IOBuffer* buf,
                        int buf_len,
                        const CompletionCallback& callback);

  void WriteDataInternal(int index,
                         int offset,
                         net::IOBuffer* buf,
                         int buf_len,
                         const CompletionCallback& callback,
                         bool truncate);

  void ReadSparseDataInternal(int64 sparse_offset,
                              net::IOBuffer* buf,
                              int buf_len,
                              const CompletionCallback& callback);

  void WriteSparseDataInternal(int64 sparse_offset,
                               net::IOBuffer* buf,
                               int buf_len,
                               const CompletionCallback& callback);

  void GetAvailableRangeInternal(int64 sparse_offset,
                                 int len,
                                 int64* out_start,
                                 const CompletionCallback& callback);

  void DoomEntryInternal(const CompletionCallback& callback);

  
  
  
  
  void CreationOperationComplete(
      const CompletionCallback& completion_callback,
      const base::TimeTicks& start_time,
      scoped_ptr<SimpleEntryCreationResults> in_results,
      Entry** out_entry,
      net::NetLog::EventType end_event_type);

  // Called after we've closed and written the EOF record to our entry. Until
  
  
  void CloseOperationComplete();

  
  
  void EntryOperationComplete(const CompletionCallback& completion_callback,
                              const SimpleEntryStat& entry_stat,
                              scoped_ptr<int> result);

  
  void ReadOperationComplete(int stream_index,
                             int offset,
                             const CompletionCallback& completion_callback,
                             scoped_ptr<uint32> read_crc32,
                             scoped_ptr<SimpleEntryStat> entry_stat,
                             scoped_ptr<int> result);

  
  void WriteOperationComplete(int stream_index,
                              const CompletionCallback& completion_callback,
                              scoped_ptr<SimpleEntryStat> entry_stat,
                              scoped_ptr<int> result);

  void ReadSparseOperationComplete(
      const CompletionCallback& completion_callback,
      scoped_ptr<base::Time> last_used,
      scoped_ptr<int> result);

  void WriteSparseOperationComplete(
      const CompletionCallback& completion_callback,
      scoped_ptr<SimpleEntryStat> entry_stat,
      scoped_ptr<int> result);

  void GetAvailableRangeOperationComplete(
      const CompletionCallback& completion_callback,
      scoped_ptr<int> result);

  
  void DoomOperationComplete(const CompletionCallback& callback,
                             State state_to_restore,
                             int result);

  
  
  
  void ChecksumOperationComplete(
      int stream_index,
      int orig_result,
      const CompletionCallback& completion_callback,
      scoped_ptr<int> result);

  
  
  
  void UpdateDataFromEntryStat(const SimpleEntryStat& entry_stat);

  int64 GetDiskUsage() const;

  
  void RecordReadIsParallelizable(const SimpleEntryOperation& operation) const;
  void RecordWriteDependencyType(const SimpleEntryOperation& operation) const;

  
  int ReadStream0Data(net::IOBuffer* buf, int offset, int buf_len);

  
  
  // + |buf_len| before being written.
  int SetStream0Data(net::IOBuffer* buf,
                     int offset, int buf_len,
                     bool truncate);

  
  
  void AdvanceCrc(net::IOBuffer* buffer,
                  int offset,
                  int length,
                  int stream_index);

  
  
  base::ThreadChecker io_thread_checker_;

  const base::WeakPtr<SimpleBackendImpl> backend_;
  const net::CacheType cache_type_;
  const scoped_refptr<base::TaskRunner> worker_pool_;
  const base::FilePath path_;
  const uint64 entry_hash_;
  const bool use_optimistic_operations_;
  std::string key_;

  
  
  
  base::Time last_used_;
  base::Time last_modified_;
  int32 data_size_[kSimpleEntryStreamCount];
  int32 sparse_data_size_;

  
  
  
  int open_count_;

  bool doomed_;

  State state_;

  
  
  
  
  int32 crc32s_end_offset_[kSimpleEntryStreamCount];
  uint32 crc32s_[kSimpleEntryStreamCount];

  // If |have_written_[index]| is true, we have written to the file that
  
  bool have_written_[kSimpleEntryStreamCount];

  
  
  CheckCrcResult crc_check_state_[kSimpleEntryStreamCount];

  
  
  
  
  
  
  SimpleSynchronousEntry* synchronous_entry_;

  std::queue<SimpleEntryOperation> pending_operations_;

  net::BoundNetLog net_log_;

  scoped_ptr<SimpleEntryOperation> executing_operation_;

  
  
  
  // |stream_0_data_| is written to the disk.
  
  
  
  
  
  scoped_refptr<net::GrowableIOBuffer> stream_0_data_;
};

}  

#endif  

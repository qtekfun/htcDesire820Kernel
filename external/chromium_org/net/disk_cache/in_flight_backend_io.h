// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_IN_FLIGHT_BACKEND_IO_H_
#define NET_DISK_CACHE_IN_FLIGHT_BACKEND_IO_H_

#include <list>
#include <string>

#include "base/message_loop/message_loop_proxy.h"
#include "base/time/time.h"
#include "net/base/completion_callback.h"
#include "net/base/io_buffer.h"
#include "net/disk_cache/in_flight_io.h"

namespace disk_cache {

class BackendImpl;
class Entry;
class EntryImpl;

class BackendIO : public BackgroundIO {
 public:
  BackendIO(InFlightIO* controller, BackendImpl* backend,
            const net::CompletionCallback& callback);

  
  void ExecuteOperation();

  
  void OnIOComplete(int result);

  
  
  void OnDone(bool cancel);

  
  bool IsEntryOperation();

  net::CompletionCallback callback() const { return callback_; }

  
  void ReferenceEntry();

  
  void Init();
  void OpenEntry(const std::string& key, Entry** entry);
  void CreateEntry(const std::string& key, Entry** entry);
  void DoomEntry(const std::string& key);
  void DoomAllEntries();
  void DoomEntriesBetween(const base::Time initial_time,
                          const base::Time end_time);
  void DoomEntriesSince(const base::Time initial_time);
  void OpenNextEntry(void** iter, Entry** next_entry);
  void OpenPrevEntry(void** iter, Entry** prev_entry);
  void EndEnumeration(void* iterator);
  void OnExternalCacheHit(const std::string& key);
  void CloseEntryImpl(EntryImpl* entry);
  void DoomEntryImpl(EntryImpl* entry);
  void FlushQueue();  
  void RunTask(const base::Closure& task);
  void ReadData(EntryImpl* entry, int index, int offset, net::IOBuffer* buf,
                int buf_len);
  void WriteData(EntryImpl* entry, int index, int offset, net::IOBuffer* buf,
                 int buf_len, bool truncate);
  void ReadSparseData(EntryImpl* entry, int64 offset, net::IOBuffer* buf,
                      int buf_len);
  void WriteSparseData(EntryImpl* entry, int64 offset, net::IOBuffer* buf,
                       int buf_len);
  void GetAvailableRange(EntryImpl* entry, int64 offset, int len, int64* start);
  void CancelSparseIO(EntryImpl* entry);
  void ReadyForSparseIO(EntryImpl* entry);

 private:
  
  
  
  
  
  enum Operation {
    OP_NONE = 0,
    OP_INIT,
    OP_OPEN,
    OP_CREATE,
    OP_DOOM,
    OP_DOOM_ALL,
    OP_DOOM_BETWEEN,
    OP_DOOM_SINCE,
    OP_OPEN_NEXT,
    OP_OPEN_PREV,
    OP_END_ENUMERATION,
    OP_ON_EXTERNAL_CACHE_HIT,
    OP_CLOSE_ENTRY,
    OP_DOOM_ENTRY,
    OP_FLUSH_QUEUE,
    OP_RUN_TASK,
    OP_MAX_BACKEND,
    OP_READ,
    OP_WRITE,
    OP_READ_SPARSE,
    OP_WRITE_SPARSE,
    OP_GET_RANGE,
    OP_CANCEL_IO,
    OP_IS_READY
  };

  virtual ~BackendIO();

  
  bool ReturnsEntry();

  
  base::TimeDelta ElapsedTime() const;

  void ExecuteBackendOperation();
  void ExecuteEntryOperation();

  BackendImpl* backend_;
  net::CompletionCallback callback_;
  Operation operation_;

  
  std::string key_;
  Entry** entry_ptr_;
  base::Time initial_time_;
  base::Time end_time_;
  void** iter_ptr_;
  void* iter_;
  EntryImpl* entry_;
  int index_;
  int offset_;
  scoped_refptr<net::IOBuffer> buf_;
  int buf_len_;
  bool truncate_;
  int64 offset64_;
  int64* start_;
  base::TimeTicks start_time_;
  base::Closure task_;

  DISALLOW_COPY_AND_ASSIGN(BackendIO);
};

class InFlightBackendIO : public InFlightIO {
 public:
  InFlightBackendIO(BackendImpl* backend,
                    base::MessageLoopProxy* background_thread);
  virtual ~InFlightBackendIO();

  
  void Init(const net::CompletionCallback& callback);
  void OpenEntry(const std::string& key, Entry** entry,
                 const net::CompletionCallback& callback);
  void CreateEntry(const std::string& key, Entry** entry,
                   const net::CompletionCallback& callback);
  void DoomEntry(const std::string& key,
                 const net::CompletionCallback& callback);
  void DoomAllEntries(const net::CompletionCallback& callback);
  void DoomEntriesBetween(const base::Time initial_time,
                          const base::Time end_time,
                          const net::CompletionCallback& callback);
  void DoomEntriesSince(const base::Time initial_time,
                        const net::CompletionCallback& callback);
  void OpenNextEntry(void** iter, Entry** next_entry,
                     const net::CompletionCallback& callback);
  void OpenPrevEntry(void** iter, Entry** prev_entry,
                     const net::CompletionCallback& callback);
  void EndEnumeration(void* iterator);
  void OnExternalCacheHit(const std::string& key);
  void CloseEntryImpl(EntryImpl* entry);
  void DoomEntryImpl(EntryImpl* entry);
  void FlushQueue(const net::CompletionCallback& callback);
  void RunTask(const base::Closure& task,
               const net::CompletionCallback& callback);
  void ReadData(EntryImpl* entry, int index, int offset, net::IOBuffer* buf,
                int buf_len, const net::CompletionCallback& callback);
  void WriteData(
      EntryImpl* entry, int index, int offset, net::IOBuffer* buf,
      int buf_len, bool truncate, const net::CompletionCallback& callback);
  void ReadSparseData(EntryImpl* entry, int64 offset, net::IOBuffer* buf,
                      int buf_len, const net::CompletionCallback& callback);
  void WriteSparseData(EntryImpl* entry, int64 offset, net::IOBuffer* buf,
                       int buf_len, const net::CompletionCallback& callback);
  void GetAvailableRange(EntryImpl* entry, int64 offset, int len, int64* start,
                         const net::CompletionCallback& callback);
  void CancelSparseIO(EntryImpl* entry);
  void ReadyForSparseIO(EntryImpl* entry,
                        const net::CompletionCallback& callback);

  
  void WaitForPendingIO();

  scoped_refptr<base::MessageLoopProxy> background_thread() {
    return background_thread_;
  }

  
  bool BackgroundIsCurrentThread() {
    return background_thread_->BelongsToCurrentThread();
  }

  base::WeakPtr<InFlightBackendIO> GetWeakPtr();

 protected:
  virtual void OnOperationComplete(BackgroundIO* operation,
                                   bool cancel) OVERRIDE;

 private:
  void PostOperation(BackendIO* operation);

  BackendImpl* backend_;
  scoped_refptr<base::MessageLoopProxy> background_thread_;
  base::WeakPtrFactory<InFlightBackendIO> ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(InFlightBackendIO);
};

}  

#endif  

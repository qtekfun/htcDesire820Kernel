// Copyright (c) 2006-2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_DISK_CACHE_DISK_CACHE_H_
#define NET_DISK_CACHE_DISK_CACHE_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/time.h"
#include "net/base/cache_type.h"
#include "net/base/completion_callback.h"

class FilePath;

namespace base {
class MessageLoopProxy;
}

namespace net {
class IOBuffer;
class NetLog;
}

namespace disk_cache {

class Entry;
class Backend;
typedef net::CompletionCallback CompletionCallback;

int CreateCacheBackend(net::CacheType type, const FilePath& path, int max_bytes,
                       bool force, base::MessageLoopProxy* thread,
                       net::NetLog* net_log, Backend** backend,
                       CompletionCallback* callback);

class Backend {
 public:
  
  
  
  
  
  
  virtual ~Backend() {}

  
  virtual int32 GetEntryCount() const = 0;

  
  
  
  
  
  
  virtual int OpenEntry(const std::string& key, Entry** entry,
                        CompletionCallback* callback) = 0;

  
  
  
  
  
  
  virtual int CreateEntry(const std::string& key, Entry** entry,
                          CompletionCallback* callback) = 0;

  
  
  
  virtual int DoomEntry(const std::string& key,
                        CompletionCallback* callback) = 0;

  
  
  
  virtual int DoomAllEntries(CompletionCallback* callback) = 0;

  
  
  
  
  virtual int DoomEntriesBetween(const base::Time initial_time,
                                 const base::Time end_time,
                                 CompletionCallback* callback) = 0;

  
  
  
  virtual int DoomEntriesSince(const base::Time initial_time,
                               CompletionCallback* callback) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  virtual int OpenNextEntry(void** iter, Entry** next_entry,
                            CompletionCallback* callback) = 0;

  
  
  
  
  virtual void EndEnumeration(void** iter) = 0;

  
  virtual void GetStats(
      std::vector<std::pair<std::string, std::string> >* stats) = 0;
};

class Entry {
 public:
  
  virtual void Doom() = 0;

  
  
  
  virtual void Close() = 0;

  
  virtual std::string GetKey() const = 0;

  
  virtual base::Time GetLastUsed() const = 0;

  
  virtual base::Time GetLastModified() const = 0;

  
  virtual int32 GetDataSize(int index) const = 0;

  
  
  
  
  
  
  
  
  
  
  
  virtual int ReadData(int index, int offset, net::IOBuffer* buf, int buf_len,
                       CompletionCallback* completion_callback) = 0;

  
  
  
  
  // number of bytes written or a network error code. If a completion callback
  
  
  
  
  
  
  
  
  virtual int WriteData(int index, int offset, net::IOBuffer* buf, int buf_len,
                        CompletionCallback* completion_callback,
                        bool truncate) = 0;

  
  
  
  // track of which parts of the entry have been written before. The backend
  // will never return data that was not written previously, so reading from
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  
  virtual int ReadSparseData(int64 offset, net::IOBuffer* buf, int buf_len,
                             CompletionCallback* completion_callback) = 0;

  
  
  
  
  
  
  virtual int WriteSparseData(int64 offset, net::IOBuffer* buf, int buf_len,
                              CompletionCallback* completion_callback) = 0;

  
  
  
  
  
  
  
  
  
  virtual int GetAvailableRange(int64 offset, int len, int64* start,
                                CompletionCallback* callback) = 0;

  
  
  
  
  
  
  
  
  virtual bool CouldBeSparse() const = 0;

  
  
  
  virtual void CancelSparseIO() = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int ReadyForSparseIO(CompletionCallback* completion_callback) = 0;

 protected:
  virtual ~Entry() {}
};

}  

#endif  

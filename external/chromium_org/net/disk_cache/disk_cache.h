// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_DISK_CACHE_DISK_CACHE_H_
#define NET_DISK_CACHE_DISK_CACHE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/time/time.h"
#include "net/base/cache_type.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"

namespace base {
class FilePath;
class MessageLoopProxy;
}

namespace net {
class IOBuffer;
class NetLog;
}

namespace disk_cache {

class Entry;
class Backend;

NET_EXPORT int CreateCacheBackend(net::CacheType type,
                                  net::BackendType backend_type,
                                  const base::FilePath& path,
                                  int max_bytes,
                                  bool force,
                                  base::MessageLoopProxy* thread,
                                  net::NetLog* net_log,
                                  scoped_ptr<Backend>* backend,
                                  const net::CompletionCallback& callback);

class NET_EXPORT Backend {
 public:
  typedef net::CompletionCallback CompletionCallback;

  
  
  
  
  
  
  virtual ~Backend() {}

  
  virtual net::CacheType GetCacheType() const = 0;

  
  virtual int32 GetEntryCount() const = 0;

  
  
  
  
  
  
  virtual int OpenEntry(const std::string& key, Entry** entry,
                        const CompletionCallback& callback) = 0;

  
  
  
  
  
  
  virtual int CreateEntry(const std::string& key, Entry** entry,
                          const CompletionCallback& callback) = 0;

  
  
  
  virtual int DoomEntry(const std::string& key,
                        const CompletionCallback& callback) = 0;

  
  
  
  virtual int DoomAllEntries(const CompletionCallback& callback) = 0;

  
  
  
  
  virtual int DoomEntriesBetween(base::Time initial_time,
                                 base::Time end_time,
                                 const CompletionCallback& callback) = 0;

  
  
  
  virtual int DoomEntriesSince(base::Time initial_time,
                               const CompletionCallback& callback) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int OpenNextEntry(void** iter, Entry** next_entry,
                            const CompletionCallback& callback) = 0;

  
  
  
  
  virtual void EndEnumeration(void** iter) = 0;

  
  virtual void GetStats(
      std::vector<std::pair<std::string, std::string> >* stats) = 0;

  
  
  virtual void OnExternalCacheHit(const std::string& key) = 0;
};

class NET_EXPORT Entry {
 public:
  typedef net::CompletionCallback CompletionCallback;
  typedef net::IOBuffer IOBuffer;

  
  virtual void Doom() = 0;

  
  
  
  virtual void Close() = 0;

  
  virtual std::string GetKey() const = 0;

  
  virtual base::Time GetLastUsed() const = 0;

  
  virtual base::Time GetLastModified() const = 0;

  
  virtual int32 GetDataSize(int index) const = 0;

  
  
  
  
  
  
  
  
  
  virtual int ReadData(int index, int offset, IOBuffer* buf, int buf_len,
                       const CompletionCallback& callback) = 0;

  
  // Returns the number of bytes written or a network error code. If this
  
  
  
  
  
  
  
  
  
  virtual int WriteData(int index, int offset, IOBuffer* buf, int buf_len,
                        const CompletionCallback& callback,
                        bool truncate) = 0;

  
  
  
  // track of which parts of the entry have been written before. The backend
  // will never return data that was not written previously, so reading from
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  
  virtual int ReadSparseData(int64 offset, IOBuffer* buf, int buf_len,
                             const CompletionCallback& callback) = 0;

  
  
  
  
  
  
  virtual int WriteSparseData(int64 offset, IOBuffer* buf, int buf_len,
                              const CompletionCallback& callback) = 0;

  
  
  
  
  
  
  
  
  
  virtual int GetAvailableRange(int64 offset, int len, int64* start,
                                const CompletionCallback& callback) = 0;

  
  
  
  
  
  
  
  
  virtual bool CouldBeSparse() const = 0;

  
  
  
  virtual void CancelSparseIO() = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int ReadyForSparseIO(const CompletionCallback& callback) = 0;

 protected:
  virtual ~Entry() {}
};

struct EntryDeleter {
  void operator()(Entry* entry) {
    
    entry->Close();
  }
};

typedef scoped_ptr<Entry, EntryDeleter> ScopedEntryPtr;

}  

#endif  

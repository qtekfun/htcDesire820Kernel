// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_FLASH_ENTRY_IMPL_H_
#define NET_DISK_CACHE_FLASH_ENTRY_IMPL_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"
#include "net/disk_cache/disk_cache.h"
#include "net/disk_cache/flash/internal_entry.h"

namespace base {

class MessageLoopProxy;

}  

namespace disk_cache {

class InternalEntry;
class IOBuffer;
class LogStore;

class NET_EXPORT_PRIVATE FlashEntryImpl
    : public Entry,
      public base::RefCountedThreadSafe<FlashEntryImpl> {
  friend class base::RefCountedThreadSafe<FlashEntryImpl>;
 public:
  FlashEntryImpl(const std::string& key,
                 LogStore* store,
                 base::MessageLoopProxy* cache_thread);
  FlashEntryImpl(int32 id,
                 LogStore* store,
                 base::MessageLoopProxy* cache_thread);

  int Init(const CompletionCallback& callback);

  
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
  virtual void CancelSparseIO() OVERRIDE;
  virtual int ReadyForSparseIO(const CompletionCallback& callback) OVERRIDE;

 private:
  void OnInitComplete(scoped_ptr<KeyAndStreamSizes> key_and_stream_sizes);
  virtual ~FlashEntryImpl();

  bool init_;
  std::string key_;
  int stream_sizes_[kFlashLogStoreEntryNumStreams];

  
  scoped_refptr<InternalEntry> new_internal_entry_;

  
  scoped_refptr<InternalEntry> old_internal_entry_;

  
  CompletionCallback callback_;

  scoped_refptr<base::MessageLoopProxy> cache_thread_;

  DISALLOW_COPY_AND_ASSIGN(FlashEntryImpl);
};

}  

#endif  

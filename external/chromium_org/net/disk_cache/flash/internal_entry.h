// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_FLASH_INTERNAL_ENTRY_H_
#define NET_DISK_CACHE_FLASH_INTERNAL_ENTRY_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"
#include "net/disk_cache/flash/format.h"

namespace net {

class IOBuffer;

}  

namespace disk_cache {

struct KeyAndStreamSizes {
  KeyAndStreamSizes();
  std::string key;
  int stream_sizes[kFlashLogStoreEntryNumStreams];
};

class LogStore;
class LogStoreEntry;

class NET_EXPORT_PRIVATE InternalEntry
    : public base::RefCountedThreadSafe<InternalEntry> {
  friend class base::RefCountedThreadSafe<InternalEntry>;
 public:
  InternalEntry(const std::string& key, LogStore* store);
  InternalEntry(int32 id, LogStore* store);

  scoped_ptr<KeyAndStreamSizes> Init();
  int32 GetDataSize(int index) const;
  int ReadData(int index, int offset, net::IOBuffer* buf, int buf_len,
               const net::CompletionCallback& callback);
  int WriteData(int index, int offset, net::IOBuffer* buf, int buf_len,
                const net::CompletionCallback& callback);
  void Close();

 private:
  bool WriteKey(LogStoreEntry* entry, const std::string& key);
  bool ReadKey(LogStoreEntry* entry, std::string* key);
  ~InternalEntry();

  LogStore* store_;
  scoped_ptr<LogStoreEntry> entry_;

  DISALLOW_COPY_AND_ASSIGN(InternalEntry);
};

}  

#endif  

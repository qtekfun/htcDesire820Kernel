// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_DUMP_CACHE_CACHE_DUMPER_H_
#define NET_TOOLS_DUMP_CACHE_CACHE_DUMPER_H_

#include <string>

#include "base/files/file_path.h"
#include "net/disk_cache/backend_impl.h"

#ifdef WIN32
#define WIN32_LARGE_FILENAME_SUPPORT
#endif

class CacheDumpWriter {
 public:
  virtual ~CacheDumpWriter() {}

  // Creates an entry to be written.
  
  
  virtual int CreateEntry(const std::string& key,
                          disk_cache::Entry** entry,
                          const net::CompletionCallback& callback) = 0;

  
  
  virtual int WriteEntry(disk_cache::Entry* entry, int stream, int offset,
                         net::IOBuffer* buf, int buf_len,
                         const net::CompletionCallback& callback) = 0;

  
  virtual void CloseEntry(disk_cache::Entry* entry, base::Time last_used,
                          base::Time last_modified) = 0;
};

class CacheDumper : public CacheDumpWriter {
 public:
  explicit CacheDumper(disk_cache::Backend* cache);

  virtual int CreateEntry(const std::string& key, disk_cache::Entry** entry,
                          const net::CompletionCallback& callback) OVERRIDE;
  virtual int WriteEntry(disk_cache::Entry* entry, int stream, int offset,
                         net::IOBuffer* buf, int buf_len,
                         const net::CompletionCallback& callback) OVERRIDE;
  virtual void CloseEntry(disk_cache::Entry* entry, base::Time last_used,
                          base::Time last_modified) OVERRIDE;

 private:
  disk_cache::Backend* cache_;
};

class DiskDumper : public CacheDumpWriter {
 public:
  explicit DiskDumper(const base::FilePath& path);

  virtual int CreateEntry(const std::string& key, disk_cache::Entry** entry,
                          const net::CompletionCallback& callback) OVERRIDE;
  virtual int WriteEntry(disk_cache::Entry* entry, int stream, int offset,
                         net::IOBuffer* buf, int buf_len,
                         const net::CompletionCallback& callback) OVERRIDE;
  virtual void CloseEntry(disk_cache::Entry* entry, base::Time last_used,
                          base::Time last_modified) OVERRIDE;

 private:
  base::FilePath path_;
  
  
  
  base::FilePath entry_path_;
  std::string entry_url_;
#ifdef WIN32_LARGE_FILENAME_SUPPORT
  HANDLE entry_;
#else
  FILE* entry_;
#endif
};

#endif  

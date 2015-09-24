// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_BASE_FILE_STREAM_H_
#define NET_BASE_FILE_STREAM_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "base/platform_file.h"
#include "net/base/completion_callback.h"

class FilePath;

namespace net {

enum Whence {
  FROM_BEGIN   = 0,
  FROM_CURRENT = 1,
  FROM_END     = 2
};

class FileStream {
 public:
  FileStream();

  
  
  
  
  
  
  FileStream(base::PlatformFile file, int flags);

  ~FileStream();

  
  
  
  void Close();

  
  
  
  
  int Open(const FilePath& path, int open_flags);

  
  bool IsOpen() const;

  
  
  
  
  int64 Seek(Whence whence, int64 offset);

  
  
  int64 Available();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int Read(char* buf, int buf_len, CompletionCallback* callback);

  
  
  
  
  
  int ReadUntilComplete(char *buf, int buf_len);

  
  // buf_len bytes will be written from buf. (In other words, partial writes are
  // allowed.)  Returns the number of bytes written, or an error code if the
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int Write(const char* buf, int buf_len, CompletionCallback* callback);

  
  
  
  
  
  int64 Truncate(int64 bytes);

  
  // written out to disk and is not currently sitting in the buffer. This does
  
  
  
  
  
  
  int Flush();

 private:
  class AsyncContext;
  friend class AsyncContext;

  
  
  scoped_ptr<AsyncContext> async_context_;

  base::PlatformFile file_;
  int open_flags_;
  bool auto_closed_;

  DISALLOW_COPY_AND_ASSIGN(FileStream);
};

}  

#endif  

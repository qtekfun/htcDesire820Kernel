// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_FILE_STREAM_WRITER_H_
#define WEBKIT_BROWSER_FILEAPI_FILE_STREAM_WRITER_H_

#include "base/basictypes.h"
#include "net/base/completion_callback.h"
#include "webkit/browser/webkit_storage_browser_export.h"

namespace base {
class FilePath;
class TaskRunner;
}

namespace net {
class IOBuffer;
}

namespace fileapi {

class FileStreamWriter {
 public:
  
  
  WEBKIT_STORAGE_BROWSER_EXPORT static FileStreamWriter* CreateForLocalFile(
      base::TaskRunner* task_runner,
      const base::FilePath& file_path,
      int64 initial_offset);

  
  
  virtual ~FileStreamWriter() {}

  
  
  // Up to buf_len bytes will be written.  (In other words, partial
  
  // the number of bytes written. If the operation could not be performed, it
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int Write(net::IOBuffer* buf, int buf_len,
                    const net::CompletionCallback& callback) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  virtual int Cancel(const net::CompletionCallback& callback) = 0;

  // Flushes the data written so far.
  
  
  
  
  
  
  
  virtual int Flush(const net::CompletionCallback& callback) = 0;
};

}  

#endif  

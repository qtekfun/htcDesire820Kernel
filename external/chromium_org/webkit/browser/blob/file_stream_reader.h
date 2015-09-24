// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BLOB_FILE_STREAM_READER_H_
#define WEBKIT_BLOB_FILE_STREAM_READER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "net/base/completion_callback.h"
#include "webkit/browser/webkit_storage_browser_export.h"

namespace base {
class FilePath;
class TaskRunner;
class Time;
}

namespace net {
class IOBuffer;
}

namespace fileapi {
class FileSystemContext;
class FileSystemURL;
}

namespace webkit_blob {

class FileStreamReader {
 public:
  
  
  
  
  
  
  
  
  
  WEBKIT_STORAGE_BROWSER_EXPORT static FileStreamReader*
      CreateForLocalFile(base::TaskRunner* task_runner,
                         const base::FilePath& file_path,
                         int64 initial_offset,
                         const base::Time& expected_modification_time);

  
  
  
  
  
  WEBKIT_STORAGE_BROWSER_EXPORT static FileStreamReader*
      CreateForFileSystemFile(fileapi::FileSystemContext* context,
                              const fileapi::FileSystemURL& url,
                              int64 initial_offset,
                              const base::Time& expected_modification_time);

  
  
  virtual ~FileStreamReader() {}

  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int Read(net::IOBuffer* buf, int buf_len,
                   const net::CompletionCallback& callback) = 0;

  
  
  
  
  
  
  
  
  
  virtual int64 GetLength(const net::Int64CompletionCallback& callback) = 0;
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_BASE_FILE_STREAM_H_
#define NET_BASE_FILE_STREAM_H_

#include "base/platform_file.h"
#include "base/task_runner.h"
#include "net/base/completion_callback.h"
#include "net/base/file_stream_whence.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"

namespace base {
class FilePath;
}

namespace net {

class IOBuffer;

class NET_EXPORT FileStream {
 public:
  
  
  
  FileStream(net::NetLog* net_log,
             const scoped_refptr<base::TaskRunner>& task_runner);

  
  explicit FileStream(net::NetLog* net_log);

  
  
  
  
  
  
  
  
  
  FileStream(base::PlatformFile file,
             int flags,
             net::NetLog* net_log,
             const scoped_refptr<base::TaskRunner>& task_runner);

  
  FileStream(base::PlatformFile file, int flags, net::NetLog* net_log);

  
  virtual ~FileStream();

  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int Open(const base::FilePath& path, int open_flags,
                   const CompletionCallback& callback);

  
  
  
  
  
  
  
  virtual int OpenSync(const base::FilePath& path, int open_flags);

  
  
  
  
  virtual int Close(const CompletionCallback& callback);

  
  
  virtual int CloseSync();

  
  virtual bool IsOpen() const;

  
  
  
  
  
  
  virtual int Seek(Whence whence, int64 offset,
                   const Int64CompletionCallback& callback);

  
  
  
  
  virtual int64 SeekSync(Whence whence, int64 offset);

  
  
  virtual int64 Available();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int Read(IOBuffer* buf, int buf_len,
                   const CompletionCallback& callback);

  
  
  
  
  
  
  
  
  virtual int ReadSync(char* buf, int buf_len);

  
  
  
  
  
  virtual int ReadUntilComplete(char *buf, int buf_len);

  
  // asynchronously.  Up to buf_len bytes will be written from buf. (In
  
  // bytes written, or an error code if the operation could not be
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int Write(IOBuffer* buf, int buf_len,
                    const CompletionCallback& callback);

  
  // synchronously.  Up to buf_len bytes will be written from buf. (In
  
  // bytes written, or an error code if the operation could not be
  
  
  
  
  
  
  virtual int WriteSync(const char* buf, int buf_len);

  
  
  
  
  
  virtual int64 Truncate(int64 bytes);

  
  // written out to disk and is not currently sitting in the buffer. This does
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int Flush(const CompletionCallback& callback);

  
  // written out to disk and is not currently sitting in the buffer. This does
  
  
  
  
  
  
  virtual int FlushSync();

  
  void EnableErrorStatistics();

  
  
  
  
  
  void SetBoundNetLogSource(const net::BoundNetLog& owner_bound_net_log);

  
  base::PlatformFile GetPlatformFileForTesting();

 private:
  class Context;

  bool is_async() const { return !!(open_flags_ & base::PLATFORM_FILE_ASYNC); }

  int open_flags_;
  net::BoundNetLog bound_net_log_;

  
  
  
  
  
  
  scoped_ptr<Context> context_;

  DISALLOW_COPY_AND_ASSIGN(FileStream);
};

}  

#endif  

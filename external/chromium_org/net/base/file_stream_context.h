// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_BASE_FILE_STREAM_CONTEXT_H_
#define NET_BASE_FILE_STREAM_CONTEXT_H_

#include "base/message_loop/message_loop.h"
#include "base/platform_file.h"
#include "base/task_runner.h"
#include "net/base/completion_callback.h"
#include "net/base/file_stream.h"
#include "net/base/file_stream_metrics.h"
#include "net/base/file_stream_whence.h"
#include "net/base/net_log.h"

#if defined(OS_POSIX)
#include <errno.h>
#endif

namespace base {
class FilePath;
}

namespace net {

class IOBuffer;

#if defined(OS_WIN)
class FileStream::Context : public base::MessageLoopForIO::IOHandler {
#elif defined(OS_POSIX)
class FileStream::Context {
#endif
 public:
  
  
  
  

  Context(const BoundNetLog& bound_net_log,
          const scoped_refptr<base::TaskRunner>& task_runner);
  Context(base::PlatformFile file,
          const BoundNetLog& bound_net_log,
          int open_flags,
          const scoped_refptr<base::TaskRunner>& task_runner);
#if defined(OS_WIN)
  virtual ~Context();
#elif defined(OS_POSIX)
  ~Context();
#endif

  int64 GetFileSize() const;

  int ReadAsync(IOBuffer* buf,
                int buf_len,
                const CompletionCallback& callback);
  int ReadSync(char* buf, int buf_len);

  int WriteAsync(IOBuffer* buf,
                 int buf_len,
                 const CompletionCallback& callback);
  int WriteSync(const char* buf, int buf_len);

  int Truncate(int64 bytes);

  
  
  

  void set_record_uma(bool value) { record_uma_ = value; }
  base::PlatformFile file() const { return file_; }
  bool async_in_progress() const { return async_in_progress_; }

  
  
  

  
  
  
  void Orphan();

  void OpenAsync(const base::FilePath& path,
                 int open_flags,
                 const CompletionCallback& callback);
  int OpenSync(const base::FilePath& path, int open_flags);

  void CloseSync();

  void CloseAsync(const CompletionCallback& callback);

  void SeekAsync(Whence whence,
                 int64 offset,
                 const Int64CompletionCallback& callback);
  int64 SeekSync(Whence whence, int64 offset);

  void FlushAsync(const CompletionCallback& callback);
  int FlushSync();

 private:
  
  
  

  struct IOResult {
    IOResult();
    IOResult(int64 result, int os_error);
    static IOResult FromOSError(int64 os_error);

    int64 result;
    int os_error;  
  };

  struct OpenResult {
    OpenResult();
    OpenResult(base::PlatformFile file, IOResult error_code);
    base::PlatformFile file;
    IOResult error_code;
  };

  
  void RecordError(const IOResult& result, FileErrorSource source) const;

  void BeginOpenEvent(const base::FilePath& path);

  OpenResult OpenFileImpl(const base::FilePath& path, int open_flags);

  void ProcessOpenError(const IOResult& result);
  void OnOpenCompleted(const CompletionCallback& callback,
                       OpenResult open_result);

  void CloseAndDelete();
  void OnCloseCompleted();

  Int64CompletionCallback IntToInt64(const CompletionCallback& callback);

  
  
  void ProcessAsyncResult(const Int64CompletionCallback& callback,
                          FileErrorSource source,
                          const IOResult& result);

  
  
  void OnAsyncCompleted(const Int64CompletionCallback& callback, int64 result);

  
  
  
  
  
  

#if defined(OS_WIN)
  int GetLastErrno() { return GetLastError(); }
  void OnAsyncFileOpened();
#elif defined(OS_POSIX)
  int GetLastErrno() { return errno; }
  void OnAsyncFileOpened() {}
  void CancelIo(base::PlatformFile) {}
#endif

  
  
  
  

  
  IOResult SeekFileImpl(Whence whence, int64 offset);

  // Flushes all data written to the stream.
  IOResult FlushFileImpl();

  
  IOResult CloseFileImpl();

#if defined(OS_WIN)
  void IOCompletionIsPending(const CompletionCallback& callback, IOBuffer* buf);

  
  virtual void OnIOCompleted(base::MessageLoopForIO::IOContext* context,
                             DWORD bytes_read,
                             DWORD error) OVERRIDE;
#elif defined(OS_POSIX)
  
  
  IOResult ReadFileImpl(scoped_refptr<IOBuffer> buf, int buf_len);

  
  
  
  IOResult WriteFileImpl(scoped_refptr<IOBuffer> buf, int buf_len);
#endif

  base::PlatformFile file_;
  bool record_uma_;
  bool async_in_progress_;
  bool orphaned_;
  BoundNetLog bound_net_log_;
  scoped_refptr<base::TaskRunner> task_runner_;

#if defined(OS_WIN)
  base::MessageLoopForIO::IOContext io_context_;
  CompletionCallback callback_;
  scoped_refptr<IOBuffer> in_flight_buf_;
  FileErrorSource error_source_;
#endif

  DISALLOW_COPY_AND_ASSIGN(Context);
};

}  

#endif  


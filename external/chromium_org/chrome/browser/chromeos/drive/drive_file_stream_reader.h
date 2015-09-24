// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_DRIVE_FILE_STREAM_READER_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_DRIVE_FILE_STREAM_READER_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "chrome/browser/chromeos/drive/file_errors.h"
#include "google_apis/drive/gdata_errorcode.h"
#include "net/base/completion_callback.h"

namespace base {
class SequencedTaskRunner;
}  

namespace net {
class HttpByteRange;
class IOBuffer;
}  

namespace drive {
namespace util {
class LocalFileReader;
}  

namespace internal {

class ReaderProxy {
 public:
  virtual ~ReaderProxy() {}

  
  virtual int Read(net::IOBuffer* buffer, int buffer_length,
                   const net::CompletionCallback& callback) = 0;

  
  virtual void OnGetContent(scoped_ptr<std::string> data) = 0;

  
  virtual void OnCompleted(FileError error) = 0;
};

class LocalReaderProxy : public ReaderProxy {
 public:
  
  
  
  
  LocalReaderProxy(
      scoped_ptr<util::LocalFileReader> file_reader, int64 length);
  virtual ~LocalReaderProxy();

  
  virtual int Read(net::IOBuffer* buffer, int buffer_length,
                   const net::CompletionCallback& callback) OVERRIDE;
  virtual void OnGetContent(scoped_ptr<std::string> data) OVERRIDE;
  virtual void OnCompleted(FileError error) OVERRIDE;

 private:
  scoped_ptr<util::LocalFileReader> file_reader_;

  
  void OnReadCompleted(
      const net::CompletionCallback& callback, int read_result);

  
  int64 remaining_length_;

  
  
  base::WeakPtrFactory<LocalReaderProxy> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(LocalReaderProxy);
};

class NetworkReaderProxy : public ReaderProxy {
 public:
  
  
  
  NetworkReaderProxy(
      int64 offset, int64 content_length, const base::Closure& job_canceller);
  virtual ~NetworkReaderProxy();

  
  virtual int Read(net::IOBuffer* buffer, int buffer_length,
                   const net::CompletionCallback& callback) OVERRIDE;
  virtual void OnGetContent(scoped_ptr<std::string> data) OVERRIDE;
  virtual void OnCompleted(FileError error) OVERRIDE;

 private:
  
  ScopedVector<std::string> pending_data_;

  
  int64 remaining_offset_;

  
  
  int64 remaining_content_length_;

  int error_code_;

  
  scoped_refptr<net::IOBuffer> buffer_;
  int buffer_length_;
  net::CompletionCallback callback_;

  
  
  
  base::Closure job_canceller_;

  DISALLOW_COPY_AND_ASSIGN(NetworkReaderProxy);
};

}  

class FileSystemInterface;
class ResourceEntry;

class DriveFileStreamReader {
 public:
  
  
  
  
  typedef base::Callback<FileSystemInterface*()> FileSystemGetter;

  
  
  typedef base::Callback<void(int error, scoped_ptr<ResourceEntry> entry)>
      InitializeCompletionCallback;

  DriveFileStreamReader(const FileSystemGetter& file_system_getter,
                        base::SequencedTaskRunner* file_task_runner);
  ~DriveFileStreamReader();

  
  bool IsInitialized() const;

  
  
  void Initialize(const base::FilePath& drive_file_path,
                  const net::HttpByteRange& byte_range,
                  const InitializeCompletionCallback& callback);

  
  
  
  
  
  
  
  
  int Read(net::IOBuffer* buffer, int buffer_length,
           const net::CompletionCallback& callback);

 private:
  
  
  void InitializeAfterGetFileContentInitialized(
      const net::HttpByteRange& in_byte_range,
      const InitializeCompletionCallback& callback,
      FileError error,
      scoped_ptr<ResourceEntry> entry,
      const base::FilePath& local_cache_file_path,
      const base::Closure& cancel_download_closure);

  
  void InitializeAfterLocalFileOpen(
      int64 length,
      const InitializeCompletionCallback& callback,
      scoped_ptr<ResourceEntry> entry,
      scoped_ptr<util::LocalFileReader> file_reader,
      int open_result);

  
  void OnGetContent(google_apis::GDataErrorCode error_code,
                    scoped_ptr<std::string> data);

  
  void OnGetFileContentCompletion(
      const InitializeCompletionCallback& callback,
      FileError error);

  const FileSystemGetter file_system_getter_;
  scoped_refptr<base::SequencedTaskRunner> file_task_runner_;
  scoped_ptr<internal::ReaderProxy> reader_proxy_;

  
  
  base::WeakPtrFactory<DriveFileStreamReader> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(DriveFileStreamReader);
};

}  

#endif  

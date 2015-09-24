// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_WEBKIT_FILE_STREAM_READER_IMPL_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_WEBKIT_FILE_STREAM_READER_IMPL_H_

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "chrome/browser/chromeos/drive/drive_file_stream_reader.h"
#include "net/base/completion_callback.h"
#include "webkit/browser/blob/file_stream_reader.h"

namespace base {
class SequencedTaskRunner;
}  

namespace drive {

class ResourceEntry;

namespace internal {

class WebkitFileStreamReaderImpl : public webkit_blob::FileStreamReader {
 public:
  WebkitFileStreamReaderImpl(
      const DriveFileStreamReader::FileSystemGetter& file_system_getter,
      base::SequencedTaskRunner* file_task_runner,
      const base::FilePath& drive_file_path,
      int64 offset,
      const base::Time& expected_modification_time);
  virtual ~WebkitFileStreamReaderImpl();

  
  virtual int Read(net::IOBuffer* buffer,
                   int buffer_length,
                   const net::CompletionCallback& callback) OVERRIDE;
  virtual int64 GetLength(const net::Int64CompletionCallback& callback)
      OVERRIDE;

 private:
  
  
  
  void OnStreamReaderInitialized(
      const net::CompletionCallback& callback,
      int error,
      scoped_ptr<ResourceEntry> entry);

  
  void ReadAfterStreamReaderInitialized(
      scoped_refptr<net::IOBuffer> buffer,
      int buffer_length,
      const net::CompletionCallback& callback,
      int initialization_result);

  
  
  void GetLengthAfterStreamReaderInitialized(
      const net::Int64CompletionCallback& callback,
      int initialization_result);

  scoped_ptr<DriveFileStreamReader> stream_reader_;
  const base::FilePath drive_file_path_;
  const int64 offset_;
  const base::Time expected_modification_time_;

  
  int64 file_size_;

  
  
  base::WeakPtrFactory<WebkitFileStreamReaderImpl> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(WebkitFileStreamReaderImpl);
};

}  
}  

#endif  

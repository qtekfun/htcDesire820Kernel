// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_FETCHER_RESPONSE_WRITER_H_
#define NET_URL_REQUEST_URL_FETCHER_RESPONSE_WRITER_H_

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"

namespace base {
class SequencedTaskRunner;
}  

namespace net {

class DrainableIOBuffer;
class FileStream;
class IOBuffer;
class URLFetcherFileWriter;
class URLFetcherStringWriter;

class NET_EXPORT URLFetcherResponseWriter {
 public:
  virtual ~URLFetcherResponseWriter() {}

  
  
  // Initialize() success results in discarding already written data.
  virtual int Initialize(const CompletionCallback& callback) = 0;

  
  // written or an error code. If ERR_IO_PENDING is returned, |callback| will be
  
  virtual int Write(IOBuffer* buffer,
                    int num_bytes,
                    const CompletionCallback& callback) = 0;

  
  
  virtual int Finish(const CompletionCallback& callback) = 0;

  
  virtual URLFetcherStringWriter* AsStringWriter();

  
  virtual URLFetcherFileWriter* AsFileWriter();
};

class NET_EXPORT URLFetcherStringWriter : public URLFetcherResponseWriter {
 public:
  URLFetcherStringWriter();
  virtual ~URLFetcherStringWriter();

  const std::string& data() const { return data_; }

  
  virtual int Initialize(const CompletionCallback& callback) OVERRIDE;
  virtual int Write(IOBuffer* buffer,
                    int num_bytes,
                    const CompletionCallback& callback) OVERRIDE;
  virtual int Finish(const CompletionCallback& callback) OVERRIDE;
  virtual URLFetcherStringWriter* AsStringWriter() OVERRIDE;

 private:
  std::string data_;

  DISALLOW_COPY_AND_ASSIGN(URLFetcherStringWriter);
};

class NET_EXPORT URLFetcherFileWriter : public URLFetcherResponseWriter {
 public:
  
  
  URLFetcherFileWriter(
      scoped_refptr<base::SequencedTaskRunner> file_task_runner,
      const base::FilePath& file_path);
  virtual ~URLFetcherFileWriter();

  const base::FilePath& file_path() const { return file_path_; }

  
  virtual int Initialize(const CompletionCallback& callback) OVERRIDE;
  virtual int Write(IOBuffer* buffer,
                    int num_bytes,
                    const CompletionCallback& callback) OVERRIDE;
  virtual int Finish(const CompletionCallback& callback) OVERRIDE;
  virtual URLFetcherFileWriter* AsFileWriter() OVERRIDE;

  
  
  void DisownFile();

 private:
  
  void DidWrite(const CompletionCallback& callback, int result);

  
  void CloseAndDeleteFile();

  
  void DidCreateTempFile(const CompletionCallback& callback,
                         base::FilePath* temp_file_path,
                         bool success);

  
  void DidOpenFile(const CompletionCallback& callback,
                   int result);

  
  void CloseComplete(const CompletionCallback& callback, int result);

  
  base::WeakPtrFactory<URLFetcherFileWriter> weak_factory_;

  
  scoped_refptr<base::SequencedTaskRunner> file_task_runner_;

  
  
  base::FilePath file_path_;

  
  bool owns_file_;

  scoped_ptr<FileStream> file_stream_;

  DISALLOW_COPY_AND_ASSIGN(URLFetcherFileWriter);
};

}  

#endif  

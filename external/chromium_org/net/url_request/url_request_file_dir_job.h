// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_FILE_DIR_JOB_H_
#define NET_URL_REQUEST_URL_REQUEST_FILE_DIR_JOB_H_

#include <string>

#include "base/files/file_path.h"
#include "base/memory/weak_ptr.h"
#include "net/base/directory_lister.h"
#include "net/url_request/url_request_job.h"

namespace net {

class URLRequestFileDirJob
  : public URLRequestJob,
    public DirectoryLister::DirectoryListerDelegate {
 public:
  URLRequestFileDirJob(URLRequest* request,
                       NetworkDelegate* network_delegate,
                       const base::FilePath& dir_path);

  bool list_complete() const { return list_complete_; }

  virtual void StartAsync();

  
  virtual void Start() OVERRIDE;
  virtual void Kill() OVERRIDE;
  virtual bool ReadRawData(IOBuffer* buf,
                           int buf_size,
                           int* bytes_read) OVERRIDE;
  virtual bool GetMimeType(std::string* mime_type) const OVERRIDE;
  virtual bool GetCharset(std::string* charset) OVERRIDE;

  
  virtual void OnListFile(
      const DirectoryLister::DirectoryListerData& data) OVERRIDE;
  virtual void OnListDone(int error) OVERRIDE;

 private:
  virtual ~URLRequestFileDirJob();

  void CloseLister();

  
  
  
  void CompleteRead();

  
  bool FillReadBuffer(char *buf, int buf_size, int *bytes_read);

  DirectoryLister lister_;
  base::FilePath dir_path_;
  std::string data_;
  bool canceled_;

  
  bool list_complete_;

  // Indicates whether we have written the HTML header
  bool wrote_header_;

  
  
  
  bool read_pending_;
  scoped_refptr<IOBuffer> read_buffer_;
  int read_buffer_length_;
  base::WeakPtrFactory<URLRequestFileDirJob> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(URLRequestFileDirJob);
};

}  

#endif  

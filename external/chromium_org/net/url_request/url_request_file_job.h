// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_FILE_JOB_H_
#define NET_URL_REQUEST_URL_REQUEST_FILE_JOB_H_

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "net/base/net_export.h"
#include "net/http/http_byte_range.h"
#include "net/url_request/url_request.h"
#include "net/url_request/url_request_job.h"

namespace base{
struct PlatformFileInfo;
class TaskRunner;
}
namespace file_util {
struct FileInfo;
}

namespace net {

class FileStream;

class NET_EXPORT URLRequestFileJob : public URLRequestJob {
 public:
  URLRequestFileJob(URLRequest* request,
                    NetworkDelegate* network_delegate,
                    const base::FilePath& file_path,
                    const scoped_refptr<base::TaskRunner>& file_task_runner);

  
  virtual void Start() OVERRIDE;
  virtual void Kill() OVERRIDE;
  virtual bool ReadRawData(IOBuffer* buf,
                           int buf_size,
                           int* bytes_read) OVERRIDE;
  virtual bool IsRedirectResponse(GURL* location,
                                  int* http_status_code) OVERRIDE;
  virtual Filter* SetupFilter() const OVERRIDE;
  virtual bool GetMimeType(std::string* mime_type) const OVERRIDE;
  virtual void SetExtraRequestHeaders(
      const HttpRequestHeaders& headers) OVERRIDE;

 protected:
  virtual ~URLRequestFileJob();

  
  base::FilePath file_path_;

 private:
  
  
  
  struct FileMetaInfo {
    FileMetaInfo();

    
    int64 file_size;
    
    std::string mime_type;
    
    
    bool mime_type_result;
    
    bool file_exists;
    
    bool is_directory;
  };

  
  static void FetchMetaInfo(const base::FilePath& file_path,
                            FileMetaInfo* meta_info);

  
  void DidFetchMetaInfo(const FileMetaInfo* meta_info);

  
  void DidOpen(int result);

  
  
  void DidSeek(int64 result);

  
  void DidRead(int result);

  scoped_ptr<FileStream> stream_;
  FileMetaInfo meta_info_;
  const scoped_refptr<base::TaskRunner> file_task_runner_;

  HttpByteRange byte_range_;
  int64 remaining_bytes_;

  base::WeakPtrFactory<URLRequestFileJob> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(URLRequestFileJob);
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DOWNLOAD_SAVE_FILE_RESOURCE_HANDLER_H_
#define CONTENT_BROWSER_DOWNLOAD_SAVE_FILE_RESOURCE_HANDLER_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "content/browser/loader/resource_handler.h"
#include "url/gurl.h"

namespace content {
class SaveFileManager;

class SaveFileResourceHandler : public ResourceHandler {
 public:
  SaveFileResourceHandler(int render_process_host_id,
                          int render_view_id,
                          const GURL& url,
                          SaveFileManager* manager);
  virtual ~SaveFileResourceHandler();

  
  virtual bool OnUploadProgress(int request_id,
                                uint64 position,
                                uint64 size) OVERRIDE;

  
  
  virtual bool OnRequestRedirected(int request_id,
                                   const GURL& url,
                                   ResourceResponse* response,
                                   bool* defer) OVERRIDE;

  
  virtual bool OnResponseStarted(int request_id,
                                 ResourceResponse* response,
                                 bool* defer) OVERRIDE;

  
  virtual bool OnWillStart(int request_id,
                           const GURL& url,
                           bool* defer) OVERRIDE;

  
  
  virtual bool OnWillRead(int request_id,
                          scoped_refptr<net::IOBuffer>* buf,
                          int* buf_size,
                          int min_size) OVERRIDE;

  
  virtual bool OnReadCompleted(int request_id, int bytes_read,
                               bool* defer) OVERRIDE;

  virtual void OnResponseCompleted(int request_id,
                                   const net::URLRequestStatus& status,
                                   const std::string& security_info,
                                   bool* defer) OVERRIDE;

  
  virtual void OnDataDownloaded(int request_id, int bytes_downloaded) OVERRIDE;

  
  
  
  void set_content_length(const std::string& content_length);

  void set_content_disposition(const std::string& content_disposition) {
    content_disposition_ = content_disposition;
  }

 private:
  int save_id_;
  int render_process_id_;
  int render_view_id_;
  scoped_refptr<net::IOBuffer> read_buffer_;
  std::string content_disposition_;
  GURL url_;
  GURL final_url_;
  int64 content_length_;
  SaveFileManager* save_manager_;

  static const int kReadBufSize = 32768;  

  DISALLOW_COPY_AND_ASSIGN(SaveFileResourceHandler);
};

}  

#endif  

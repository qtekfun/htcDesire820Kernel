// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_LOADER_REDIRECT_TO_FILE_RESOURCE_HANDLER_H_
#define CONTENT_BROWSER_LOADER_REDIRECT_TO_FILE_RESOURCE_HANDLER_H_

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/platform_file.h"
#include "content/browser/loader/layered_resource_handler.h"
#include "net/url_request/url_request.h"
#include "net/url_request/url_request_status.h"

namespace net {
class FileStream;
class GrowableIOBuffer;
}

namespace webkit_blob {
class ShareableFileReference;
}

namespace content {
class ResourceDispatcherHostImpl;

class RedirectToFileResourceHandler : public LayeredResourceHandler {
 public:
  RedirectToFileResourceHandler(
      scoped_ptr<ResourceHandler> next_handler,
      net::URLRequest* request,
      ResourceDispatcherHostImpl* resource_dispatcher_host);
  virtual ~RedirectToFileResourceHandler();

  
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
  virtual bool OnReadCompleted(int request_id,
                               int bytes_read,
                               bool* defer) OVERRIDE;
  virtual void OnResponseCompleted(int request_id,
                                   const net::URLRequestStatus& status,
                                   const std::string& security_info,
                                   bool* defer) OVERRIDE;

 private:
  void DidCreateTemporaryFile(base::PlatformFileError error_code,
                              base::PassPlatformFile file_handle,
                              const base::FilePath& file_path);
  void DidWriteToFile(int result);
  bool WriteMore();
  bool BufIsFull() const;
  void ResumeIfDeferred();

  base::WeakPtrFactory<RedirectToFileResourceHandler> weak_factory_;

  ResourceDispatcherHostImpl* host_;

  
  
  
  
  
  

  scoped_refptr<net::GrowableIOBuffer> buf_;
  bool buf_write_pending_;
  int write_cursor_;

  scoped_ptr<net::FileStream> file_stream_;
  bool write_callback_pending_;

  
  
  
  
  
  int next_buffer_size_;

  
  
  scoped_refptr<webkit_blob::ShareableFileReference> deletable_file_;

  bool did_defer_ ;

  bool completed_during_write_;
  net::URLRequestStatus completed_status_;
  std::string completed_security_info_;

  DISALLOW_COPY_AND_ASSIGN(RedirectToFileResourceHandler);
};

}  

#endif  

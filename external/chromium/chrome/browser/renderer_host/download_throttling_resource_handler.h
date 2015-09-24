// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_RENDERER_HOST_DOWNLOAD_THROTTLING_RESOURCE_HANDLER_H_
#define CHROME_BROWSER_RENDERER_HOST_DOWNLOAD_THROTTLING_RESOURCE_HANDLER_H_
#pragma once

#include <string>

#include "chrome/browser/download/download_request_limiter.h"
#include "content/browser/renderer_host/resource_handler.h"
#include "googleurl/src/gurl.h"

class DownloadResourceHandler;
class ResourceDispatcherHost;

namespace net {
class URLRequest;
}  


class DownloadThrottlingResourceHandler
    : public ResourceHandler,
      public DownloadRequestLimiter::Callback {
 public:
  DownloadThrottlingResourceHandler(ResourceDispatcherHost* host,
                                    net::URLRequest* request,
                                    const GURL& url,
                                    int render_process_host_id,
                                    int render_view_id,
                                    int request_id,
                                    bool in_complete);

  
  virtual bool OnUploadProgress(int request_id,
                                uint64 position,
                                uint64 size);
  virtual bool OnRequestRedirected(int request_id, const GURL& url,
                                   ResourceResponse* response, bool* defer);
  virtual bool OnResponseStarted(int request_id, ResourceResponse* response);
  virtual bool OnWillStart(int request_id, const GURL& url, bool* defer);
  virtual bool OnWillRead(int request_id, net::IOBuffer** buf, int* buf_size,
                          int min_size);
  virtual bool OnReadCompleted(int request_id, int* bytes_read);
  virtual bool OnResponseCompleted(int request_id,
                                   const net::URLRequestStatus& status,
                                   const std::string& security_info);
  virtual void OnRequestClosed();

  
  virtual void CancelDownload();
  virtual void ContinueDownload();

 private:
  virtual ~DownloadThrottlingResourceHandler();

  void CopyTmpBufferToDownloadHandler();

  ResourceDispatcherHost* host_;
  net::URLRequest* request_;
  GURL url_;
  int render_process_host_id_;
  int render_view_id_;
  int request_id_;

  
  
  scoped_refptr<DownloadResourceHandler> download_handler_;

  
  
  scoped_refptr<ResourceResponse> response_;

  
  
  scoped_refptr<net::IOBuffer> tmp_buffer_;
  int tmp_buffer_length_;

  
  
  
  
  bool ignore_on_read_complete_;

  
  
  bool request_closed_;

  DISALLOW_COPY_AND_ASSIGN(DownloadThrottlingResourceHandler);
};

#endif  

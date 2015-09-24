// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_RENDERER_HOST_DOWNLOAD_RESOURCE_HANDLER_H_
#define CHROME_BROWSER_RENDERER_HOST_DOWNLOAD_RESOURCE_HANDLER_H_
#pragma once

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/timer.h"
#include "chrome/browser/download/download_file.h"
#include "content/browser/renderer_host/global_request_id.h"
#include "content/browser/renderer_host/resource_handler.h"

class DownloadFileManager;
class ResourceDispatcherHost;
struct DownloadBuffer;

namespace net {
class URLRequest;
}  

class DownloadResourceHandler : public ResourceHandler {
 public:
  DownloadResourceHandler(ResourceDispatcherHost* rdh,
                          int render_process_host_id,
                          int render_view_id,
                          int request_id,
                          const GURL& url,
                          DownloadFileManager* download_file_manager,
                          net::URLRequest* request,
                          bool save_as,
                          const DownloadSaveInfo& save_info);

  virtual bool OnUploadProgress(int request_id, uint64 position, uint64 size);

  
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

  
  
  
  void set_content_length(const int64& content_length);

  void set_content_disposition(const std::string& content_disposition);

  void CheckWriteProgress();

  std::string DebugString() const;

 private:
  ~DownloadResourceHandler();

  void StartPauseTimer();

  int download_id_;
  GlobalRequestID global_id_;
  int render_view_id_;
  scoped_refptr<net::IOBuffer> read_buffer_;
  std::string content_disposition_;
  int64 content_length_;
  DownloadFileManager* download_file_manager_;
  net::URLRequest* request_;
  bool save_as_;  
  DownloadSaveInfo save_info_;
  scoped_ptr<DownloadBuffer> buffer_;
  ResourceDispatcherHost* rdh_;
  bool is_paused_;
  base::OneShotTimer<DownloadResourceHandler> pause_timer_;
  base::TimeTicks download_start_time_;  
  static const int kReadBufSize = 32768;  
  static const size_t kLoadsToWrite = 100;  
  static const int kThrottleTimeMs = 200;  

  DISALLOW_COPY_AND_ASSIGN(DownloadResourceHandler);
};

#endif  
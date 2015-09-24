// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DOWNLOAD_DOWNLOAD_RESOURCE_HANDLER_H_
#define CONTENT_BROWSER_DOWNLOAD_DOWNLOAD_RESOURCE_HANDLER_H_

#include <string>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer/timer.h"
#include "content/browser/loader/resource_handler.h"
#include "content/public/browser/download_manager.h"
#include "content/public/browser/download_save_info.h"
#include "content/public/browser/download_url_parameters.h"
#include "content/public/browser/global_request_id.h"
#include "net/base/net_errors.h"


namespace net {
class URLRequest;
}  

namespace content {
class ByteStreamWriter;
class ByteStreamReader;
class DownloadRequestHandle;
struct DownloadCreateInfo;

class CONTENT_EXPORT DownloadResourceHandler
    : public ResourceHandler,
      public base::SupportsWeakPtr<DownloadResourceHandler> {
 public:
  
  
  static const int kDownloadByteStreamSize;

  
  
  DownloadResourceHandler(
      uint32 id,
      net::URLRequest* request,
      const DownloadUrlParameters::OnStartedCallback& started_cb,
      scoped_ptr<DownloadSaveInfo> save_info);

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

  void PauseRequest();
  void ResumeRequest();
  void CancelRequest();

  std::string DebugString() const;

 private:
  virtual ~DownloadResourceHandler();

  
  
  
  void CallStartedCB(DownloadItem* item, net::Error error);

  uint32 download_id_;
  
  
  DownloadUrlParameters::OnStartedCallback started_cb_;
  scoped_ptr<DownloadSaveInfo> save_info_;

  
  scoped_refptr<net::IOBuffer> read_buffer_;       
  scoped_ptr<ByteStreamWriter> stream_writer_; 

  
  base::TimeTicks download_start_time_;
  base::TimeTicks last_read_time_;
  base::TimeTicks last_stream_pause_time_;
  base::TimeDelta total_pause_time_;
  size_t last_buffer_size_;
  int64 bytes_read_;

  int pause_count_;
  bool was_deferred_;

  
  bool on_response_started_called_;

  static const int kReadBufSize = 32768;  
  static const int kThrottleTimeMs = 200;  

  DISALLOW_COPY_AND_ASSIGN(DownloadResourceHandler);
};

}  

#endif  

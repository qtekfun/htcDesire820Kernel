// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_LOADER_BUFFERED_RESOURCE_HANDLER_H_
#define CONTENT_BROWSER_LOADER_BUFFERED_RESOURCE_HANDLER_H_

#include <string>
#include <vector>

#include "base/memory/weak_ptr.h"
#include "content/browser/loader/layered_resource_handler.h"
#include "content/public/browser/resource_controller.h"

namespace net {
class URLRequest;
}

namespace content {
class ResourceDispatcherHostImpl;
struct WebPluginInfo;

class BufferedResourceHandler
    : public LayeredResourceHandler,
      public ResourceController {
 public:
  BufferedResourceHandler(scoped_ptr<ResourceHandler> next_handler,
                          ResourceDispatcherHostImpl* host,
                          net::URLRequest* request);
  virtual ~BufferedResourceHandler();

 private:
  
  virtual void SetController(ResourceController* controller) OVERRIDE;
  virtual bool OnResponseStarted(int request_id,
                                 ResourceResponse* response,
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

  
  virtual void Resume() OVERRIDE;
  virtual void Cancel() OVERRIDE;
  virtual void CancelAndIgnore() OVERRIDE;
  virtual void CancelWithError(int error_code) OVERRIDE;

  bool ProcessResponse(bool* defer);

  bool ShouldSniffContent();
  bool DetermineMimeType();
  bool SelectNextHandler(bool* defer);
  bool UseAlternateNextHandler(scoped_ptr<ResourceHandler> handler);

  bool ReplayReadCompleted(bool* defer);
  void CallReplayReadCompleted();

  bool MustDownload();
  bool HasSupportingPlugin(bool* is_stale);

  
  bool CopyReadBufferToNextHandler(int request_id);

  
  void OnPluginsLoaded(const std::vector<WebPluginInfo>& plugins);

  enum State {
    STATE_STARTING,

    
    
    STATE_BUFFERING,

    
    
    
    
    STATE_PROCESSING,

    
    
    STATE_REPLAYING,

    
    STATE_STREAMING
  };
  State state_;

  scoped_refptr<ResourceResponse> response_;
  ResourceDispatcherHostImpl* host_;
  scoped_refptr<net::IOBuffer> read_buffer_;
  int read_buffer_size_;
  int bytes_read_;

  bool must_download_;
  bool must_download_is_set_;

  base::WeakPtrFactory<BufferedResourceHandler> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(BufferedResourceHandler);
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_LOADER_CROSS_SITE_RESOURCE_HANDLER_H_
#define CONTENT_BROWSER_LOADER_CROSS_SITE_RESOURCE_HANDLER_H_

#include "base/memory/ref_counted.h"
#include "content/browser/loader/layered_resource_handler.h"
#include "net/url_request/url_request_status.h"

namespace net {
class URLRequest;
}

namespace content {

class CrossSiteResourceHandler : public LayeredResourceHandler {
 public:
  CrossSiteResourceHandler(scoped_ptr<ResourceHandler> next_handler,
                           net::URLRequest* request);
  virtual ~CrossSiteResourceHandler();

  
  virtual bool OnRequestRedirected(int request_id,
                                   const GURL& new_url,
                                   ResourceResponse* response,
                                   bool* defer) OVERRIDE;
  virtual bool OnResponseStarted(int request_id,
                                 ResourceResponse* response,
                                 bool* defer) OVERRIDE;
  virtual bool OnReadCompleted(int request_id,
                               int bytes_read,
                               bool* defer) OVERRIDE;
  virtual void OnResponseCompleted(int request_id,
                                   const net::URLRequestStatus& status,
                                   const std::string& security_info,
                                   bool* defer) OVERRIDE;

  
  
  void ResumeResponse();

 private:
  
  
  void StartCrossSiteTransition(int request_id,
                                ResourceResponse* response,
                                bool should_transfer);

  void ResumeIfDeferred();

  bool has_started_response_;
  bool in_cross_site_transition_;
  bool completed_during_transition_;
  bool did_defer_;
  net::URLRequestStatus completed_status_;
  std::string completed_security_info_;
  scoped_refptr<ResourceResponse> response_;

  DISALLOW_COPY_AND_ASSIGN(CrossSiteResourceHandler);
};

}  

#endif  

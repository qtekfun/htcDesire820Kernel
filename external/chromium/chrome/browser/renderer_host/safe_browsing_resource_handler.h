// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_RENDERER_HOST_SAFE_BROWSING_RESOURCE_HANDLER_H_
#define CHROME_BROWSER_RENDERER_HOST_SAFE_BROWSING_RESOURCE_HANDLER_H_
#pragma once

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/time.h"
#include "base/timer.h"
#include "chrome/browser/safe_browsing/safe_browsing_service.h"
#include "content/browser/renderer_host/resource_handler.h"

class ResourceDispatcherHost;

class SafeBrowsingResourceHandler : public ResourceHandler,
                                    public SafeBrowsingService::Client {
 public:
  SafeBrowsingResourceHandler(ResourceHandler* handler,
                              int render_process_host_id,
                              int render_view_id,
                              ResourceType::Type resource_type,
                              SafeBrowsingService* safe_browsing,
                              ResourceDispatcherHost* resource_dispatcher_host);

  
  virtual bool OnUploadProgress(int request_id, uint64 position, uint64 size);
  virtual bool OnRequestRedirected(int request_id, const GURL& new_url,
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

  
  
  virtual void OnBrowseUrlCheckResult(
      const GURL& url, SafeBrowsingService::UrlCheckResult result);

  
  
  virtual void OnBlockingPageComplete(bool proceed);

 private:
  
  enum State {
    STATE_NONE,
    STATE_CHECKING_URL,
    STATE_DISPLAYING_BLOCKING_PAGE,
  };

  
  enum DeferState {
    DEFERRED_NONE,
    DEFERRED_START,
    DEFERRED_REDIRECT,
  };

  ~SafeBrowsingResourceHandler();

  
  void Shutdown();

  
  
  
  bool CheckUrl(const GURL& url);

  
  
  void OnCheckUrlTimeout();

  
  void StartDisplayingBlockingPage(const GURL& url,
                                   SafeBrowsingService::UrlCheckResult result);

  
  
  void ResumeRequest();

  
  void ResumeStart();

  
  void ResumeRedirect();

  
  
  void ClearDeferredRequestInfo();

  State state_;
  DeferState defer_state_;

  
  
  SafeBrowsingService::UrlCheckResult safe_browsing_result_;

  
  base::TimeTicks url_check_start_time_;

  
  base::OneShotTimer<SafeBrowsingResourceHandler> timer_;

  
  std::vector<GURL> redirect_urls_;

  
  
  GURL deferred_url_;
  int deferred_request_id_;
  scoped_refptr<ResourceResponse> deferred_redirect_response_;

  scoped_refptr<ResourceHandler> next_handler_;
  int render_process_host_id_;
  int render_view_id_;
  scoped_refptr<SafeBrowsingService> safe_browsing_;
  ResourceDispatcherHost* rdh_;
  ResourceType::Type resource_type_;

  DISALLOW_COPY_AND_ASSIGN(SafeBrowsingResourceHandler);
};


#endif  

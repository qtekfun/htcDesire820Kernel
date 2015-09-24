// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRERENDER_PRERENDER_RESOURCE_HANDLER_H_
#define CHROME_BROWSER_PRERENDER_PRERENDER_RESOURCE_HANDLER_H_
#pragma once

#include <string>

#include "base/callback.h"
#include "chrome/browser/prerender/prerender_manager.h"
#include "content/browser/renderer_host/resource_handler.h"

class ChromeURLRequestContext;
namespace net {
class URLRequest;
}

namespace prerender {

class PrerenderResourceHandler : public ResourceHandler {
 public:
  
  
  
  
  
  
  static PrerenderResourceHandler* MaybeCreate(
      const net::URLRequest& request,
      ChromeURLRequestContext* context,
      ResourceHandler* next_handler,
      bool is_from_prerender, int child_id, int route_id);

  
  
  
  virtual bool OnResponseStarted(int request_id,
                                 ResourceResponse* response);

  
  virtual bool OnUploadProgress(int request_id,
                                uint64 position,
                                uint64 size);
  virtual bool OnRequestRedirected(int request_id, const GURL& url,
                                   ResourceResponse* response,
                                   bool* defer);
  virtual bool OnWillStart(int request_id, const GURL& url, bool* defer);

  virtual bool OnWillRead(int request_id,
                          net::IOBuffer** buf,
                          int* buf_size,
                          int min_size);

  virtual bool OnReadCompleted(int request_id, int* bytes_read);

  virtual bool OnResponseCompleted(int request_id,
                                   const net::URLRequestStatus& status,
                                   const std::string& security_info);

  virtual void OnRequestClosed();

 private:
  friend class PrerenderResourceHandlerTest;
  typedef Callback5<const std::pair<int, int>&,
                    const GURL&,
                    const std::vector<GURL>&,
                    const GURL&,
                    bool>::Type PrerenderCallback;

  PrerenderResourceHandler(const net::URLRequest& request,
                           ResourceHandler* next_handler,
                           PrerenderManager* prerender_manager,
                           bool make_pending, int child_id, int route_id);

  
  PrerenderResourceHandler(const net::URLRequest& request,
                           ResourceHandler* next_handler,
                           PrerenderCallback* callback);

  virtual ~PrerenderResourceHandler();

  void RunCallbackFromUIThread(const std::pair<int, int>& child_route_id_pair,
                               const GURL& url,
                               const std::vector<GURL>& alias_urls,
                               const GURL& referrer,
                               bool make_pending);
  void StartPrerender(const std::pair<int, int>& child_route_id_pair,
                      const GURL& url,
                      const std::vector<GURL>& alias_urls,
                      const GURL& referrer,
                      bool make_pending);

  
  
  std::vector<GURL> alias_urls_;
  GURL url_;
  scoped_refptr<ResourceHandler> next_handler_;
  scoped_refptr<PrerenderManager> prerender_manager_;
  scoped_ptr<PrerenderCallback> prerender_callback_;

  
  
  const net::URLRequest& request_;

  int child_id_;
  int route_id_;

  
  bool make_pending_;

  DISALLOW_COPY_AND_ASSIGN(PrerenderResourceHandler);
};

}  

#endif  

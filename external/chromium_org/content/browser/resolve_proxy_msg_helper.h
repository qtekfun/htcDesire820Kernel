// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RESOLVE_PROXY_MSG_HELPER_H_
#define CONTENT_BROWSER_RESOLVE_PROXY_MSG_HELPER_H_

#include <deque>
#include <string>

#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"
#include "content/public/browser/browser_message_filter.h"
#include "net/base/completion_callback.h"
#include "net/proxy/proxy_service.h"
#include "url/gurl.h"

namespace net {
class URLRequestContextGetter;
}

namespace content {

class CONTENT_EXPORT ResolveProxyMsgHelper : public BrowserMessageFilter {
 public:
  explicit ResolveProxyMsgHelper(net::URLRequestContextGetter* getter);
  
  explicit ResolveProxyMsgHelper(net::ProxyService* proxy_service);

  
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok) OVERRIDE;

  void OnResolveProxy(const GURL& url, IPC::Message* reply_msg);

 protected:
  
  
  virtual ~ResolveProxyMsgHelper();

 private:
  
  void OnResolveProxyCompleted(int result);

  
  void StartPendingRequest();

  
  struct PendingRequest {
   public:
     PendingRequest(const GURL& url, IPC::Message* reply_msg) :
         url(url), reply_msg(reply_msg), pac_req(NULL) { }

     
     GURL url;

     
     IPC::Message* reply_msg;

     
     net::ProxyService::PacRequest* pac_req;
  };

  
  net::ProxyInfo proxy_info_;

  
  typedef std::deque<PendingRequest> PendingRequestList;
  PendingRequestList pending_requests_;

  scoped_refptr<net::URLRequestContextGetter> context_getter_;
  net::ProxyService* proxy_service_;
};

}  

#endif  

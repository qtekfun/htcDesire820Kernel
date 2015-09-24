// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_RESOLVE_PROXY_MSG_HELPER_H_
#define CHROME_BROWSER_NET_RESOLVE_PROXY_MSG_HELPER_H_
#pragma once

#include <deque>
#include <string>

#include "base/memory/ref_counted.h"
#include "content/browser/browser_message_filter.h"
#include "googleurl/src/gurl.h"
#include "net/base/completion_callback.h"
#include "net/proxy/proxy_service.h"

class ResolveProxyMsgHelper : public BrowserMessageFilter {
 public:
  
  
  explicit ResolveProxyMsgHelper(net::ProxyService* proxy_service);

  
  
  ~ResolveProxyMsgHelper();

  
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok);

  void OnResolveProxy(const GURL& url, IPC::Message* reply_msg);

 private:
  
  void OnResolveProxyCompleted(int result);

  
  void StartPendingRequest();

  
  
  bool GetProxyService(scoped_refptr<net::ProxyService>* out) const;

  
  struct PendingRequest {
   public:
     PendingRequest(const GURL& url, IPC::Message* reply_msg) :
         url(url), reply_msg(reply_msg), pac_req(NULL) { }

     
     GURL url;

     
     IPC::Message* reply_msg;

     
     net::ProxyService::PacRequest* pac_req;
  };

  
  scoped_refptr<net::ProxyService> proxy_service_;
  net::CompletionCallbackImpl<ResolveProxyMsgHelper> callback_;
  net::ProxyInfo proxy_info_;

  
  typedef std::deque<PendingRequest> PendingRequestList;
  PendingRequestList pending_requests_;

  
  
  scoped_refptr<net::ProxyService> proxy_service_override_;
};

#endif  

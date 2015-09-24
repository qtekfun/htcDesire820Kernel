// Copyright (c) 2006-2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_HTTP_PRECONNECT_H__
#define NET_HTTP_PRECONNECT_H__
#pragma once

#include "base/scoped_ptr.h"
#include "net/base/completion_callback.h"
#include "net/base/net_log.h"
#include "net/http/http_request_info.h"
#include "net/http/http_stream_factory.h"

namespace net {

class ProxyInfo;
struct SSLConfig;

class Preconnect {
 public:
  
  
  
  static void DoPreconnect(HttpNetworkSession* session, const GURL& url,
      int count = 1, HttpRequestInfo::RequestMotivation motivation =
          HttpRequestInfo::PRECONNECT_MOTIVATED);

 private:
  explicit Preconnect(HttpNetworkSession* session);
  virtual ~Preconnect();

  void OnPreconnectComplete(int error_code);

  
  
  
  void Connect(const GURL& url, int count,
      HttpRequestInfo::RequestMotivation motivation);

  HttpNetworkSession * session_;
  
  scoped_ptr<HttpRequestInfo> request_info_;

  
  scoped_ptr<SSLConfig> ssl_config_;

  
  scoped_ptr<ProxyInfo> proxy_info_;

  
  BoundNetLog net_log_;

  
  scoped_ptr<HttpStreamRequest> stream_request_;

  CompletionCallbackImpl<Preconnect> io_callback_;

  DISALLOW_COPY_AND_ASSIGN(Preconnect);
};
}  
#endif 


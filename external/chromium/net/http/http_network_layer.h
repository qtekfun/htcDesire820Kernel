// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_NETWORK_LAYER_H_
#define NET_HTTP_HTTP_NETWORK_LAYER_H_
#pragma once

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "net/base/net_export.h"
#include "net/http/http_transaction_factory.h"

namespace net {

class CertVerifier;
class ClientSocketFactory;
class DnsCertProvenanceChecker;
class DnsRRResolver;
class HostResolver;
class HttpAuthHandlerFactory;
class HttpNetworkSession;
class NetLog;
class NetworkDelegate;
class ProxyService;
class SpdySessionPool;
class SSLConfigService;
class SSLHostInfoFactory;

class NET_EXPORT HttpNetworkLayer : public HttpTransactionFactory,
                                    public base::NonThreadSafe {
 public:
  
  
  explicit HttpNetworkLayer(HttpNetworkSession* session);
  virtual ~HttpNetworkLayer();

  
  
  
  
  
  
  static HttpTransactionFactory* CreateFactory(HttpNetworkSession* session);

  
  
  
  
  
  
  
  
  static void EnableSpdy(const std::string& mode);

  
  virtual int CreateTransaction(scoped_ptr<HttpTransaction>* trans);
  virtual HttpCache* GetCache();
  virtual HttpNetworkSession* GetSession();
  virtual void Suspend(bool suspend);

 private:
  const scoped_refptr<HttpNetworkSession> session_;
  bool suspended_;
};

}  

#endif  

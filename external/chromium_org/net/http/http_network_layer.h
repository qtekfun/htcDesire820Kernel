// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_NETWORK_LAYER_H_
#define NET_HTTP_HTTP_NETWORK_LAYER_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/power_monitor/power_observer.h"
#include "base/threading/non_thread_safe.h"
#include "net/base/net_export.h"
#include "net/http/http_transaction_factory.h"

namespace net {

class HttpNetworkSession;

class NET_EXPORT HttpNetworkLayer
    : public HttpTransactionFactory,
      public base::PowerObserver,
      NON_EXPORTED_BASE(public base::NonThreadSafe) {
 public:
  
  
  explicit HttpNetworkLayer(HttpNetworkSession* session);
  virtual ~HttpNetworkLayer();

  
  
  
  
  
  
  static HttpTransactionFactory* CreateFactory(HttpNetworkSession* session);

  
  
  static void ForceAlternateProtocol();

  
  virtual int CreateTransaction(RequestPriority priority,
                                scoped_ptr<HttpTransaction>* trans,
                                HttpTransactionDelegate* delegate) OVERRIDE;
  virtual HttpCache* GetCache() OVERRIDE;
  virtual HttpNetworkSession* GetSession() OVERRIDE;

  
  virtual void OnSuspend() OVERRIDE;
  virtual void OnResume() OVERRIDE;

 private:
  const scoped_refptr<HttpNetworkSession> session_;
  bool suspended_;

  DISALLOW_COPY_AND_ASSIGN(HttpNetworkLayer);
};

}  

#endif  

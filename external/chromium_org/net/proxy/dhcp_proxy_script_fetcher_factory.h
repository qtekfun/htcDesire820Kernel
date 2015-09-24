// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_DHCP_SCRIPT_FETCHER_FACTORY_H_
#define NET_PROXY_DHCP_SCRIPT_FETCHER_FACTORY_H_

#include "base/basictypes.h"
#include "base/memory/singleton.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"

namespace net {

class DhcpProxyScriptFetcher;
class URLRequestContext;

class NET_EXPORT DhcpProxyScriptFetcherFactory {
 public:
  
  DhcpProxyScriptFetcherFactory();

  
  
  
  
  
  
  
  DhcpProxyScriptFetcher* Create(URLRequestContext* url_request_context);

  
  
  
  
  void set_enabled(bool enabled);

  
  
  bool enabled() const;

  
  
  static bool IsSupported();

 private:
  bool feature_enabled_;

  DISALLOW_COPY_AND_ASSIGN(DhcpProxyScriptFetcherFactory);
};

}  

#endif  

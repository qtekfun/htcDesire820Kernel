// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_SERVICE_V8_H_
#define NET_PROXY_PROXY_SERVICE_V8_H_

#include "base/basictypes.h"
#include "net/base/net_export.h"

namespace net {

class DhcpProxyScriptFetcher;
class HostResolver;
class NetLog;
class NetworkDelegate;
class ProxyConfigService;
class ProxyScriptFetcher;
class ProxyService;

NET_EXPORT ProxyService* CreateProxyServiceUsingV8ProxyResolver(
    ProxyConfigService* proxy_config_service,
    ProxyScriptFetcher* proxy_script_fetcher,
    DhcpProxyScriptFetcher* dhcp_proxy_script_fetcher,
    HostResolver* host_resolver,
    NetLog* net_log,
    NetworkDelegate* network_delegate);

}  

#endif  

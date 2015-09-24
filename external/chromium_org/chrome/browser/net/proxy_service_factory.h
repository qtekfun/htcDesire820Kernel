// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_PROXY_SERVICE_FACTORY_H_
#define CHROME_BROWSER_NET_PROXY_SERVICE_FACTORY_H_

#include "base/basictypes.h"

class CommandLine;
class PrefProxyConfigTracker;
class PrefService;

namespace net {
class NetLog;
class NetworkDelegate;
class ProxyConfigService;
class ProxyService;
class URLRequestContext;
}

class ProxyServiceFactory {
 public:
  
  
  static net::ProxyConfigService* CreateProxyConfigService(
      PrefProxyConfigTracker* tracker);

  
  
  
  
  static PrefProxyConfigTracker* CreatePrefProxyConfigTrackerOfProfile(
      PrefService* profile_prefs,
      PrefService* local_state_prefs);

  
  
  
  static PrefProxyConfigTracker* CreatePrefProxyConfigTrackerOfLocalState(
      PrefService* local_state_prefs);

  
  static net::ProxyService* CreateProxyService(
      net::NetLog* net_log,
      net::URLRequestContext* context,
      net::NetworkDelegate* network_delegate,
      net::ProxyConfigService* proxy_config_service,
      const CommandLine& command_line);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(ProxyServiceFactory);
};

#endif  

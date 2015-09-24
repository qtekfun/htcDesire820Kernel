// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_SSL_CONFIG_SERVICE_MANAGER_H_
#define CHROME_BROWSER_NET_SSL_CONFIG_SERVICE_MANAGER_H_

namespace net {
class SSLConfigService;
}  

class PrefService;
class PrefRegistrySimple;

class SSLConfigServiceManager {
 public:
  
  
  
  static SSLConfigServiceManager* CreateDefaultManager(
      PrefService* local_state);

  static void RegisterPrefs(PrefRegistrySimple* registry);

  virtual ~SSLConfigServiceManager() {}

  
  
  
  
  virtual net::SSLConfigService* Get() = 0;
};

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_SSL_CONFIG_SERVICE_MANAGER_H_
#define CHROME_BROWSER_NET_SSL_CONFIG_SERVICE_MANAGER_H_
#pragma once

namespace net {
class SSLConfigService;
}  

class PrefService;

class SSLConfigServiceManager {
 public:
  
  
  
  
  
  
  static SSLConfigServiceManager* CreateDefaultManager(
      PrefService* user_prefs,
      PrefService* local_state);

  virtual ~SSLConfigServiceManager() {}

  
  
  
  
  virtual net::SSLConfigService* Get() = 0;
};

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_CONFIG_SERVICE_H_
#define NET_PROXY_PROXY_CONFIG_SERVICE_H_

#include "net/base/net_export.h"

namespace net {

class ProxyConfig;

class NET_EXPORT ProxyConfigService {
 public:
  
  enum ConfigAvailability {
    
    CONFIG_PENDING,
    
    CONFIG_VALID,
    
    CONFIG_UNSET
  };

  
  class NET_EXPORT Observer {
   public:
    virtual ~Observer() {}
    
    
    
    
    
    virtual void OnProxyConfigChanged(const ProxyConfig& config,
                                      ConfigAvailability availability) = 0;
  };

  virtual ~ProxyConfigService() {}

  
  
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;

  
  // the proxy configuration is written to |config| and CONFIG_VALID is
  
  
  
  
  
  virtual ConfigAvailability GetLatestProxyConfig(ProxyConfig* config) = 0;

  
  
  
  
  
  
  
  virtual void OnLazyPoll() {}
};

}  

#endif  

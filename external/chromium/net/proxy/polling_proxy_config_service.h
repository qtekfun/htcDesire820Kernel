// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_POLLING_PROXY_CONFIG_SERVICE_H_
#define NET_PROXY_POLLING_PROXY_CONFIG_SERVICE_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "base/time.h"
#include "net/proxy/proxy_config_service.h"

namespace net {

class PollingProxyConfigService : public ProxyConfigService {
 public:
  
  virtual void AddObserver(Observer* observer);
  virtual void RemoveObserver(Observer* observer);
  virtual ConfigAvailability GetLatestProxyConfig(ProxyConfig* config);
  virtual void OnLazyPoll();

 protected:
  
  
  
  typedef void (*GetConfigFunction)(ProxyConfig*);

  
  
  
  PollingProxyConfigService(
      base::TimeDelta poll_interval,
      GetConfigFunction get_config_func);

  virtual ~PollingProxyConfigService();

  
  void CheckForChangesNow();

 private:
  class Core;
  scoped_refptr<Core> core_;
};

}  

#endif  

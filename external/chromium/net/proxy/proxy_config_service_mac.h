// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_CONFIG_SERVICE_MAC_H_
#define NET_PROXY_PROXY_CONFIG_SERVICE_MAC_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/observer_list.h"
#include "net/base/network_config_watcher_mac.h"
#include "net/proxy/proxy_config.h"
#include "net/proxy/proxy_config_service.h"

namespace net {

class ProxyConfigServiceMac : public ProxyConfigService {
 public:
  
  
  
  explicit ProxyConfigServiceMac(MessageLoop* io_loop);
  virtual ~ProxyConfigServiceMac();

 public:
  
  virtual void AddObserver(Observer* observer);
  virtual void RemoveObserver(Observer* observer);
  virtual ConfigAvailability GetLatestProxyConfig(ProxyConfig* config);

 private:
  class Helper;

  
  
  class Forwarder : public NetworkConfigWatcherMac::Delegate {
   public:
    explicit Forwarder(ProxyConfigServiceMac* net_config_watcher)
        : net_config_watcher_(net_config_watcher) {}

    
    virtual void SetDynamicStoreNotificationKeys(SCDynamicStoreRef store) {
      net_config_watcher_->SetDynamicStoreNotificationKeys(store);
    }
    virtual void OnNetworkConfigChange(CFArrayRef changed_keys) {
      net_config_watcher_->OnNetworkConfigChange(changed_keys);
    }

   private:
    ProxyConfigServiceMac* const net_config_watcher_;
    DISALLOW_COPY_AND_ASSIGN(Forwarder);
  };

  
  void SetDynamicStoreNotificationKeys(SCDynamicStoreRef store);
  void OnNetworkConfigChange(CFArrayRef changed_keys);

  
  void OnProxyConfigChanged(const ProxyConfig& new_config);

  Forwarder forwarder_;
  const NetworkConfigWatcherMac config_watcher_;

  ObserverList<Observer> observers_;

  
  bool has_fetched_config_;
  ProxyConfig last_config_fetched_;

  scoped_refptr<Helper> helper_;

  
  MessageLoop* io_loop_;

  DISALLOW_COPY_AND_ASSIGN(ProxyConfigServiceMac);
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_CONFIG_SERVICE_MAC_H_
#define NET_PROXY_PROXY_CONFIG_SERVICE_MAC_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "net/base/network_config_watcher_mac.h"
#include "net/proxy/proxy_config.h"
#include "net/proxy/proxy_config_service.h"

namespace base {
class SingleThreadTaskRunner;
}  

namespace net {

class NET_EXPORT ProxyConfigServiceMac : public ProxyConfigService {
 public:
  
  
  
  explicit ProxyConfigServiceMac(
      base::SingleThreadTaskRunner* io_thread_task_runner);
  virtual ~ProxyConfigServiceMac();

 public:
  
  virtual void AddObserver(Observer* observer) OVERRIDE;
  virtual void RemoveObserver(Observer* observer) OVERRIDE;
  virtual ConfigAvailability GetLatestProxyConfig(ProxyConfig* config) OVERRIDE;

 private:
  class Helper;

  
  
  class Forwarder : public NetworkConfigWatcherMac::Delegate {
   public:
    explicit Forwarder(ProxyConfigServiceMac* proxy_config_service)
        : proxy_config_service_(proxy_config_service) {}

    
    virtual void StartReachabilityNotifications() OVERRIDE {}
    virtual void SetDynamicStoreNotificationKeys(
        SCDynamicStoreRef store) OVERRIDE;
    virtual void OnNetworkConfigChange(CFArrayRef changed_keys) OVERRIDE;

   private:
    ProxyConfigServiceMac* const proxy_config_service_;
    DISALLOW_COPY_AND_ASSIGN(Forwarder);
  };

  
  void SetDynamicStoreNotificationKeys(SCDynamicStoreRef store);
  void OnNetworkConfigChange(CFArrayRef changed_keys);

  
  void OnProxyConfigChanged(const ProxyConfig& new_config);

  Forwarder forwarder_;
  scoped_ptr<const NetworkConfigWatcherMac> config_watcher_;

  ObserverList<Observer> observers_;

  
  bool has_fetched_config_;
  ProxyConfig last_config_fetched_;

  scoped_refptr<Helper> helper_;

  
  const scoped_refptr<base::SingleThreadTaskRunner> io_thread_task_runner_;

  DISALLOW_COPY_AND_ASSIGN(ProxyConfigServiceMac);
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_NETWORK_CONFIG_WATCHER_MAC_H_
#define NET_BASE_NETWORK_CONFIG_WATCHER_MAC_H_

#include <SystemConfiguration/SCDynamicStore.h>

#include "base/basictypes.h"
#include "base/mac/scoped_cftyperef.h"
#include "base/memory/scoped_ptr.h"

namespace base {
class Thread;
}

namespace net {

class NetworkConfigWatcherMac {
 public:
  
  
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    
    virtual void Init() {}

    
    
    
    virtual void StartReachabilityNotifications() = 0;

    
    
    
    virtual void SetDynamicStoreNotificationKeys(SCDynamicStoreRef store) = 0;

    
    
    virtual void OnNetworkConfigChange(CFArrayRef changed_keys) = 0;
  };

  explicit NetworkConfigWatcherMac(Delegate* delegate);
  ~NetworkConfigWatcherMac();

 private:
  
  
  
  scoped_ptr<base::Thread> notifier_thread_;

  DISALLOW_COPY_AND_ASSIGN(NetworkConfigWatcherMac);
};

}  

#endif  

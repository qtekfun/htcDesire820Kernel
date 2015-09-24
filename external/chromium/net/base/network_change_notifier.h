// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_NETWORK_CHANGE_NOTIFIER_H_
#define NET_BASE_NETWORK_CHANGE_NOTIFIER_H_
#pragma once

#include "base/basictypes.h"
#include "base/observer_list_threadsafe.h"
#include "net/base/net_export.h"

namespace net {

class NET_EXPORT NetworkChangeNotifier {
 public:
  class NET_EXPORT IPAddressObserver {
   public:
    virtual ~IPAddressObserver() {}

    
    
    virtual void OnIPAddressChanged() = 0;

   protected:
    IPAddressObserver() {}

   private:
    DISALLOW_COPY_AND_ASSIGN(IPAddressObserver);
  };

  class NET_EXPORT OnlineStateObserver {
   public:
    virtual ~OnlineStateObserver() {}

    
    
    
    virtual void OnOnlineStateChanged(bool online) = 0;

   protected:
    OnlineStateObserver() {}

   private:
    DISALLOW_COPY_AND_ASSIGN(OnlineStateObserver);
  };

  virtual ~NetworkChangeNotifier();

  
  
  
  virtual bool IsCurrentlyOffline() const = 0;

  
  
  
  
  
  
  static NetworkChangeNotifier* Create();

  
  
  
  
  
  
  
  static bool IsOffline();

  
  
  static NetworkChangeNotifier* CreateMock();

  
  
  
  
  
  static void AddIPAddressObserver(IPAddressObserver* observer);
  static void AddOnlineStateObserver(OnlineStateObserver* observer);

  
  
  
  
  
  
  
  static void RemoveIPAddressObserver(IPAddressObserver* observer);
  static void RemoveOnlineStateObserver(OnlineStateObserver* observer);

#ifdef UNIT_TEST
  
  static void NotifyObserversOfIPAddressChangeForTests() {
    NotifyObserversOfIPAddressChange();
  }
#endif

 protected:
  NetworkChangeNotifier();

  
  
  
  static void NotifyObserversOfIPAddressChange();
  void NotifyObserversOfOnlineStateChange();

 private:
  const scoped_refptr<ObserverListThreadSafe<IPAddressObserver> >
      ip_address_observer_list_;
  const scoped_refptr<ObserverListThreadSafe<OnlineStateObserver> >
      online_state_observer_list_;

  DISALLOW_COPY_AND_ASSIGN(NetworkChangeNotifier);
};

}  

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_PREF_PROXY_CONFIG_SERVICE_H_
#define CHROME_BROWSER_NET_PREF_PROXY_CONFIG_SERVICE_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "chrome/browser/prefs/proxy_config_dictionary.h"
#include "content/common/notification_observer.h"
#include "net/proxy/proxy_config.h"
#include "net/proxy/proxy_config_service.h"

class PrefService;
class PrefSetObserver;

class PrefProxyConfigTracker
    : public base::RefCountedThreadSafe<PrefProxyConfigTracker>,
      public NotificationObserver {
 public:
  
  
  class Observer {
   public:
    virtual ~Observer() {}
    virtual void OnPrefProxyConfigChanged() = 0;
  };

  
  enum ConfigState {
    
    CONFIG_PRESENT,
    
    CONFIG_FALLBACK,
    
    CONFIG_UNSET,
  };

  explicit PrefProxyConfigTracker(PrefService* pref_service);

  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  
  
  ConfigState GetProxyConfig(net::ProxyConfig* config);

  
  
  void DetachFromPrefService();

 private:
  friend class base::RefCountedThreadSafe<PrefProxyConfigTracker>;
  virtual ~PrefProxyConfigTracker();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  
  
  
  void InstallProxyConfig(const net::ProxyConfig& config, ConfigState state);

  
  
  
  ConfigState ReadPrefConfig(net::ProxyConfig* config);

  
  
  static bool PrefConfigToNetConfig(const ProxyConfigDictionary& proxy_dict,
                                    net::ProxyConfig* config);

  
  
  net::ProxyConfig pref_config_;

  
  
  ConfigState config_state_;

  
  ObserverList<Observer, true> observers_;

  
  PrefService* pref_service_;
  scoped_ptr<PrefSetObserver> proxy_prefs_observer_;

  DISALLOW_COPY_AND_ASSIGN(PrefProxyConfigTracker);
};

class PrefProxyConfigService
    : public net::ProxyConfigService,
      public net::ProxyConfigService::Observer,
      public PrefProxyConfigTracker::Observer {
 public:
  
  PrefProxyConfigService(PrefProxyConfigTracker* tracker,
                         net::ProxyConfigService* base_service);
  virtual ~PrefProxyConfigService();

  
  virtual void AddObserver(net::ProxyConfigService::Observer* observer);
  virtual void RemoveObserver(net::ProxyConfigService::Observer* observer);
  virtual ConfigAvailability GetLatestProxyConfig(net::ProxyConfig* config);
  virtual void OnLazyPoll();

  static void RegisterPrefs(PrefService* user_prefs);

 private:
  
  virtual void OnProxyConfigChanged(const net::ProxyConfig& config,
                                    ConfigAvailability availability);

  
  virtual void OnPrefProxyConfigChanged();

  
  
  void RegisterObservers();

  scoped_ptr<net::ProxyConfigService> base_service_;
  ObserverList<net::ProxyConfigService::Observer, true> observers_;
  scoped_refptr<PrefProxyConfigTracker> pref_config_tracker_;

  
  bool registered_observers_;

  DISALLOW_COPY_AND_ASSIGN(PrefProxyConfigService);
};

#endif  

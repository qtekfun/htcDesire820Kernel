// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_PREF_PROXY_CONFIG_TRACKER_IMPL_H_
#define CHROME_BROWSER_NET_PREF_PROXY_CONFIG_TRACKER_IMPL_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/prefs/pref_change_registrar.h"
#include "chrome/browser/net/pref_proxy_config_tracker.h"
#include "chrome/browser/prefs/proxy_config_dictionary.h"
#include "net/proxy/proxy_config.h"
#include "net/proxy/proxy_config_service.h"

class PrefService;
class PrefRegistrySimple;

namespace user_prefs {
class PrefRegistrySyncable;
}

class ChromeProxyConfigService
    : public net::ProxyConfigService,
      public net::ProxyConfigService::Observer {
 public:
  
  
  
  explicit ChromeProxyConfigService(net::ProxyConfigService* base_service);
  virtual ~ChromeProxyConfigService();

  
  virtual void AddObserver(
      net::ProxyConfigService::Observer* observer) OVERRIDE;
  virtual void RemoveObserver(
      net::ProxyConfigService::Observer* observer) OVERRIDE;
  virtual ConfigAvailability GetLatestProxyConfig(
      net::ProxyConfig* config) OVERRIDE;
  virtual void OnLazyPoll() OVERRIDE;

  
  
  void UpdateProxyConfig(ProxyPrefs::ConfigState config_state,
                         const net::ProxyConfig& config);

 private:
  
  virtual void OnProxyConfigChanged(const net::ProxyConfig& config,
                                    ConfigAvailability availability) OVERRIDE;

  
  void RegisterObserver();

  scoped_ptr<net::ProxyConfigService> base_service_;
  ObserverList<net::ProxyConfigService::Observer, true> observers_;

  
  
  ProxyPrefs::ConfigState pref_config_state_;

  
  net::ProxyConfig pref_config_;

  
  
  bool pref_config_read_pending_;

  
  bool registered_observer_;

  DISALLOW_COPY_AND_ASSIGN(ChromeProxyConfigService);
};

class PrefProxyConfigTrackerImpl : public PrefProxyConfigTracker {
 public:
  explicit PrefProxyConfigTrackerImpl(PrefService* pref_service);
  virtual ~PrefProxyConfigTrackerImpl();

  
  virtual scoped_ptr<net::ProxyConfigService> CreateTrackingProxyConfigService(
      scoped_ptr<net::ProxyConfigService> base_service) OVERRIDE;

  
  
  virtual void DetachFromPrefService() OVERRIDE;

  
  
  static bool PrefPrecedes(ProxyPrefs::ConfigState config_state);

  
  
  
  
  
  
  
  
  static net::ProxyConfigService::ConfigAvailability GetEffectiveProxyConfig(
      ProxyPrefs::ConfigState pref_state,
      const net::ProxyConfig& pref_config,
      net::ProxyConfigService::ConfigAvailability system_availability,
      const net::ProxyConfig& system_config,
      bool ignore_fallback_config,
      ProxyPrefs::ConfigState* effective_config_state,
      net::ProxyConfig* effective_config);

  
  
  static bool PrefConfigToNetConfig(const ProxyConfigDictionary& proxy_dict,
                                    net::ProxyConfig* config);

  
  
  static void RegisterPrefs(PrefRegistrySimple* registry);
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  
  
  static ProxyPrefs::ConfigState ReadPrefConfig(const PrefService* pref_service,
                                                net::ProxyConfig* config);

 protected:
  
  
  
  
  ProxyPrefs::ConfigState GetProxyConfig(net::ProxyConfig* config);

  
  
  virtual void OnProxyConfigChanged(ProxyPrefs::ConfigState config_state,
                                    const net::ProxyConfig& config);

  void OnProxyPrefChanged();

  const PrefService* prefs() const { return pref_service_; }
  bool update_pending() const { return update_pending_; }

 private:
  
  
  ProxyPrefs::ConfigState config_state_;

  
  net::ProxyConfig pref_config_;

  PrefService* pref_service_;
  ChromeProxyConfigService* chrome_proxy_config_service_;  
  bool update_pending_;  
  PrefChangeRegistrar proxy_prefs_;

  DISALLOW_COPY_AND_ASSIGN(PrefProxyConfigTrackerImpl);
};

#endif  

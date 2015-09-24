// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_PROXY_CONFIG_SERVICE_IMPL_H_
#define CHROME_BROWSER_CHROMEOS_PROXY_CONFIG_SERVICE_IMPL_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/prefs/pref_change_registrar.h"
#include "chrome/browser/net/pref_proxy_config_tracker_impl.h"
#include "chromeos/network/network_state_handler_observer.h"
#include "components/onc/onc_constants.h"

namespace chromeos {

class NetworkState;

class ProxyConfigServiceImpl : public PrefProxyConfigTrackerImpl,
                               public NetworkStateHandlerObserver {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  explicit ProxyConfigServiceImpl(PrefService* profile_prefs,
                                  PrefService* local_state_prefs);
  virtual ~ProxyConfigServiceImpl();

  
  virtual void OnProxyConfigChanged(ProxyPrefs::ConfigState config_state,
                                    const net::ProxyConfig& config) OVERRIDE;

  
  virtual void DefaultNetworkChanged(const NetworkState* network) OVERRIDE;

 protected:
  friend class UIProxyConfigService;

  
  
  
  static bool IgnoreProxy(const PrefService* profile_prefs,
                          const std::string network_profile_path,
                          onc::ONCSource onc_source);

 private:
  
  void OnProxyPrefChanged();

  
  
  
  
  void DetermineEffectiveConfigFromDefaultNetwork();

  
  
  ProxyPrefs::ConfigState active_config_state_;

  
  net::ProxyConfig active_config_;

  
  
  PrefChangeRegistrar profile_pref_registrar_;

  
  PrefChangeRegistrar local_state_pref_registrar_;

  
  
  PrefService* profile_prefs_;

  
  PrefService* local_state_prefs_;

  base::WeakPtrFactory<ProxyConfigServiceImpl> pointer_factory_;

  DISALLOW_COPY_AND_ASSIGN(ProxyConfigServiceImpl);
};

}  

#endif  

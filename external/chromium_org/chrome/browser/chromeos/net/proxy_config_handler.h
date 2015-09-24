// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_NET_PROXY_CONFIG_HANDLER_H_
#define CHROME_BROWSER_CHROMEOS_NET_PROXY_CONFIG_HANDLER_H_

#include "base/memory/scoped_ptr.h"
#include "components/onc/onc_constants.h"

class PrefRegistrySimple;
class PrefService;
class ProxyConfigDictionary;

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace chromeos {

class FavoriteState;

namespace proxy_config {

scoped_ptr<ProxyConfigDictionary> GetProxyConfigForFavoriteNetwork(
    const PrefService* profile_prefs,
    const PrefService* local_state_prefs,
    const FavoriteState& network,
    onc::ONCSource* onc_source);

void SetProxyConfigForFavoriteNetwork(const ProxyConfigDictionary& proxy_config,
                                      const FavoriteState& network);

void RegisterPrefs(PrefRegistrySimple* registry);

void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

}  

}  

#endif  

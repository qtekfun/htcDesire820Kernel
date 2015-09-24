// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_NET_ONC_UTILS_H_
#define CHROME_BROWSER_CHROMEOS_NET_ONC_UTILS_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "components/onc/onc_constants.h"

class PrefService;

namespace base {
class DictionaryValue;
class ListValue;
}

namespace chromeos {

class FavoriteState;
class NetworkState;
class User;

namespace onc {

scoped_ptr<base::DictionaryValue> ConvertOncProxySettingsToProxyConfig(
    const base::DictionaryValue& onc_proxy_settings);

void ExpandStringPlaceholdersInNetworksForUser(
    const chromeos::User* user,
    base::ListValue* network_configs);

void ImportNetworksForUser(const chromeos::User* user,
                           const base::ListValue& network_configs,
                           std::string* error);

const base::DictionaryValue* FindPolicyForActiveUser(
    const std::string& guid,
    ::onc::ONCSource* onc_source);

const base::DictionaryValue* GetGlobalConfigFromPolicy(bool for_active_user);

bool PolicyAllowsOnlyPolicyNetworksToAutoconnect(bool for_active_user);

const base::DictionaryValue* GetPolicyForFavoriteNetwork(
    const PrefService* profile_prefs,
    const PrefService* local_state_prefs,
    const FavoriteState& favorite,
    ::onc::ONCSource* onc_source);

bool HasPolicyForFavoriteNetwork(const PrefService* profile_prefs,
                                 const PrefService* local_state_prefs,
                                 const FavoriteState& network);

}  
}  

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_UI_PROXY_CONFIG_SERVICE_H_
#define CHROME_BROWSER_CHROMEOS_UI_PROXY_CONFIG_SERVICE_H_

#include <string>

#include "base/basictypes.h"
#include "chrome/browser/chromeos/ui_proxy_config.h"

class PrefService;

namespace chromeos {

class FavoriteState;

class UIProxyConfigService {
 public:
  UIProxyConfigService();
  ~UIProxyConfigService();

  
  
  
  
  void SetPrefs(PrefService* profile_prefs, PrefService* local_state_prefs);

  
  
  
  void SetCurrentNetwork(const std::string& current_network);

  void UpdateFromPrefs();

  
  
  
  void GetProxyConfig(UIProxyConfig* config) const;

  
  
  void SetProxyConfig(const UIProxyConfig& config);

 private:
  
  
  
  
  void DetermineEffectiveConfig(const FavoriteState& network);

  
  
  std::string current_ui_network_;

  
  UIProxyConfig current_ui_config_;

  bool signin_screen_;

  
  PrefService* profile_prefs_;

  
  PrefService* local_state_prefs_;

  DISALLOW_COPY_AND_ASSIGN(UIProxyConfigService);
};

}  

#endif  

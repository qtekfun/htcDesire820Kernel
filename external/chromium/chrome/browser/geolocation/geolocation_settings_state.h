// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_GEOLOCATION_GEOLOCATION_SETTINGS_STATE_H_
#define CHROME_BROWSER_GEOLOCATION_GEOLOCATION_SETTINGS_STATE_H_
#pragma once

#include <map>
#include <set>

#include "chrome/common/content_settings.h"
#include "content/browser/tab_contents/navigation_controller.h"

class GeolocationContentSettingsMap;

class GeolocationSettingsState {
 public:
  explicit GeolocationSettingsState(Profile* profile);
  virtual ~GeolocationSettingsState();

  typedef std::map<GURL, ContentSetting> StateMap;
  const StateMap& state_map() const {
    return state_map_;
  }

  
  void OnGeolocationPermissionSet(const GURL& requesting_origin, bool allowed);

  
  
  void DidNavigate(const NavigationController::LoadCommittedDetails& details);

  void ClearStateMap();

  enum TabState {
    TABSTATE_NONE = 0,
    
    TABSTATE_HAS_EXCEPTION = 1 << 1,
    
    TABSTATE_HAS_ANY_ICON = 1 << 2,
    
    TABSTATE_HAS_ANY_ALLOWED = 1 << 3,
    
    TABSTATE_HAS_CHANGED = 1 << 4,
  };

  
  typedef std::map<ContentSetting, std::set<std::string> >
      FormattedHostsPerState;

  
  void GetDetailedInfo(FormattedHostsPerState* formatted_hosts_per_state,
                       unsigned int* tab_state_flags) const;

 private:
  std::string GURLToFormattedHost(const GURL& url) const;

  Profile* profile_;
  StateMap state_map_;
  GURL embedder_url_;

  DISALLOW_COPY_AND_ASSIGN(GeolocationSettingsState);
};

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_USAGES_STATE_H_
#define CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_USAGES_STATE_H_

#include <map>
#include <set>

#include "chrome/common/content_settings.h"
#include "chrome/common/content_settings_types.h"
#include "url/gurl.h"

class Profile;

namespace content {
struct LoadCommittedDetails;
}

class ContentSettingsUsagesState {
 public:
  ContentSettingsUsagesState(Profile* profile, ContentSettingsType type);
  virtual ~ContentSettingsUsagesState();

  typedef std::map<GURL, ContentSetting> StateMap;
  const StateMap& state_map() const {
    return state_map_;
  }

  
  void OnPermissionSet(const GURL& requesting_origin, bool allowed);

  
  
  void DidNavigate(const content::LoadCommittedDetails& details);

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
  ContentSettingsType type_;
  StateMap state_map_;
  GURL embedder_url_;

  DISALLOW_COPY_AND_ASSIGN(ContentSettingsUsagesState);
};

#endif  

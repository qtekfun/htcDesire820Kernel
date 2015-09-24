// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_CLEAR_BROWSER_DATA_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_CLEAR_BROWSER_DATA_HANDLER_H_

#include "base/prefs/pref_member.h"
#include "chrome/browser/browsing_data/browsing_data_remover.h"
#include "chrome/browser/ui/webui/options/options_ui.h"

namespace options {

class ClearBrowserDataHandler : public OptionsPageUIHandler,
                                public BrowsingDataRemover::Observer {
 public:
  ClearBrowserDataHandler();
  virtual ~ClearBrowserDataHandler();

  
  virtual void GetLocalizedValues(DictionaryValue* localized_strings) OVERRIDE;
  virtual void InitializeHandler() OVERRIDE;
  virtual void InitializePage() OVERRIDE;

  
  virtual void RegisterMessages() OVERRIDE;

  void UpdateInfoBannerVisibility();

 private:
  
  void HandleClearBrowserData(const ListValue* value);

  
  
  virtual void OnBrowsingDataRemoverDone() OVERRIDE;

  
  virtual void OnBrowsingHistoryPrefChanged();

  
  
  BrowsingDataRemover* remover_;

  
  BooleanPrefMember clear_plugin_lso_data_enabled_;

  
  // settings and removal options (e.g. Content Licenses) are available.
  BooleanPrefMember pepper_flash_settings_enabled_;

  
  BooleanPrefMember allow_deleting_browser_history_;

  DISALLOW_COPY_AND_ASSIGN(ClearBrowserDataHandler);
};

}  

#endif  

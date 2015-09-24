// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_RESET_PROFILE_SETTINGS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_RESET_PROFILE_SETTINGS_HANDLER_H_

#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/ui/webui/options/options_ui.h"

namespace base {
class DictionaryValue;
class ListValue;
}  

class AutomaticProfileResetter;
class BrandcodeConfigFetcher;
class ProfileResetter;
class ResettableSettingsSnapshot;

namespace options {

class ResetProfileSettingsHandler
    : public OptionsPageUIHandler,
      public base::SupportsWeakPtr<ResetProfileSettingsHandler> {
 public:
  ResetProfileSettingsHandler();
  virtual ~ResetProfileSettingsHandler();

  
  virtual void GetLocalizedValues(
      base::DictionaryValue* localized_strings) OVERRIDE;
  virtual void InitializeHandler() OVERRIDE;
  virtual void InitializePage() OVERRIDE;
  virtual void Uninitialize() OVERRIDE;

  
  virtual void RegisterMessages() OVERRIDE;

 private:
  
  void HandleResetProfileSettings(const base::ListValue* value);

  
  void OnResetProfileSettingsDone();

  
  void OnShowResetProfileDialog(const base::ListValue* value);

  
  void OnDismissedResetProfileSettingsBanner(const base::ListValue* args);

  
  void OnSettingsFetched();

  
  
  void ResetProfile(bool send_settings);

  
  
  AutomaticProfileResetter* automatic_profile_resetter_;

  
  
  bool has_shown_confirmation_dialog_;

  scoped_ptr<ProfileResetter> resetter_;

  scoped_ptr<BrandcodeConfigFetcher> config_fetcher_;

  
  scoped_ptr<ResettableSettingsSnapshot> setting_snapshot_;

  
  std::string brandcode_;

  DISALLOW_COPY_AND_ASSIGN(ResetProfileSettingsHandler);
};

}  

#endif  

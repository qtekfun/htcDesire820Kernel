// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_MANAGE_PROFILE_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_MANAGE_PROFILE_HANDLER_H_

#include <string>

#include "base/memory/weak_ptr.h"
#include "base/prefs/pref_change_registrar.h"
#include "chrome/browser/sync/profile_sync_service_observer.h"
#include "chrome/browser/ui/webui/options/options_ui.h"

namespace base {
class StringValue;
}

namespace options {

class ManageProfileHandler : public OptionsPageUIHandler,
                             public ProfileSyncServiceObserver {
 public:
  ManageProfileHandler();
  virtual ~ManageProfileHandler();

  
  virtual void GetLocalizedValues(
      base::DictionaryValue* localized_strings) OVERRIDE;
  virtual void InitializeHandler() OVERRIDE;
  virtual void InitializePage() OVERRIDE;
  virtual void Uninitialize() OVERRIDE;

  
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void OnStateChanged() OVERRIDE;

 private:
  
  
  
  void RequestDefaultProfileIcons(const base::ListValue* args);

  
  
  
  void RequestNewProfileDefaults(const base::ListValue* args);

  
  
  
  void SendProfileIcons(const base::StringValue& icon_grid);

  
  
  
  
  
  
  
  void SendProfileNames();

  
  
  
  
  
  
  
  void SetProfileIconAndName(const base::ListValue* args);

#if defined(ENABLE_SETTINGS_APP)
  
  
  
  void SwitchAppListProfile(const base::ListValue* args);
#endif

  
  
  void ProfileIconSelectionChanged(const base::ListValue* args);

  
  
  
  
  void RequestHasProfileShortcuts(const base::ListValue* args);

  
  
  
  
  void RequestCreateProfileUpdate(const base::ListValue* args);

  
  
  void OnCreateManagedUserPrefChange();

  
  
  void OnHasProfileShortcuts(bool has_shortcuts);

  
  
  
  void AddProfileShortcut(const base::ListValue* args);

  
  
  
  void RemoveProfileShortcut(const base::ListValue* args);

  
  std::string gaia_picture_url_;

  
  
  PrefChangeRegistrar pref_change_registrar_;

  
  base::WeakPtrFactory<ManageProfileHandler> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(ManageProfileHandler);
};

}  

#endif  

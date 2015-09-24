// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_USER_CROS_SETTINGS_PROVIDER_H_
#define CHROME_BROWSER_CHROMEOS_USER_CROS_SETTINGS_PROVIDER_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "chrome/browser/chromeos/cros_settings_provider.h"
#include "chrome/browser/chromeos/login/signed_settings_helper.h"

class ListValue;
class PrefService;
class Task;

namespace chromeos {

class UserCrosSettingsProvider : public CrosSettingsProvider {
 public:
  UserCrosSettingsProvider();
  virtual ~UserCrosSettingsProvider() {}

  
  static void RegisterPrefs(PrefService* local_state);

  
  
  
  
  
  bool RequestTrustedAllowGuest(Task* callback);
  bool RequestTrustedAllowNewUser(Task* callback);
  bool RequestTrustedDataRoamingEnabled(Task* callback);
  bool RequestTrustedShowUsersOnSignin(Task* callback);
  bool RequestTrustedOwner(Task* callback);

  
  void Reload();

  
  static bool cached_allow_guest();
  static bool cached_allow_new_user();
  static bool cached_data_roaming_enabled();
  static bool cached_show_users_on_signin();
  static const ListValue* cached_whitelist();
  static std::string cached_owner();

  
  
  
  static bool IsEmailInCachedWhitelist(const std::string& email);

  
  virtual bool Get(const std::string& path, Value** out_value) const;
  virtual bool HandlesSetting(const std::string& path);

  void WhitelistUser(const std::string& email);
  void UnwhitelistUser(const std::string& email);

  
  static void UpdateCachedOwner(const std::string& email);

 private:
  
  virtual void DoSet(const std::string& path, Value* value);

  DISALLOW_COPY_AND_ASSIGN(UserCrosSettingsProvider);
};

}  

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_APPS_PROMO_H_
#define CHROME_BROWSER_EXTENSIONS_APPS_PROMO_H_
#pragma once

#include <set>
#include <string>

#include "base/gtest_prod_util.h"
#include "chrome/common/extensions/extension.h"

class PrefService;

class AppsPromo {
 public:
  
  
  static void RegisterPrefs(PrefService* local_state);
  static void RegisterUserPrefs(PrefService* prefs);

  
  static void ClearPromo();

  
  static std::string GetPromoId();

  
  static std::string GetPromoButtonText();

  
  static std::string GetPromoHeaderText();

  
  static GURL GetPromoLink();

  
  static std::string GetPromoExpireText();

  
  static void SetPromo(const std::string& id,
                       const std::string& header_text,
                       const std::string& button_text,
                       const GURL& link,
                       const std::string& expire_text);

  explicit AppsPromo(PrefService* prefs);
  ~AppsPromo();

  
  
  const ExtensionIdSet& old_default_apps() const {
    return old_default_app_ids_;
  }

  
  
  
  void ExpireDefaultApps();

  
  void HidePromo();

  
  void MaximizeAppsIfFirstView();

  
  bool ShouldShowAppLauncher(const ExtensionIdSet& installed_ids);

  
  bool ShouldShowPromo(const ExtensionIdSet& installed_ids,
                       bool* just_expired);

 private:
  FRIEND_TEST_ALL_PREFIXES(ExtensionAppsPromo, HappyPath);
  FRIEND_TEST_ALL_PREFIXES(ExtensionAppsPromo, PromoPrefs);
  FRIEND_TEST_ALL_PREFIXES(ExtensionAppsPromo, UpdatePromoFocus);

  
  
  
  static const int kDefaultAppsCounterMax;

  
  static bool IsPromoSupportedForLocale();

  bool GetDefaultAppsInstalled() const;

  
  std::string GetLastPromoId();
  void SetLastPromoId(const std::string& id);

  
  
  int GetPromoCounter() const;
  void SetPromoCounter(int val);

  
  PrefService* prefs_;

  
  
  ExtensionIdSet old_default_app_ids_;

  DISALLOW_COPY_AND_ASSIGN(AppsPromo);
};

#endif  

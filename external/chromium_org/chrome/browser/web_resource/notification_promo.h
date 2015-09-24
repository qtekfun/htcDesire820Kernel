// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_WEB_RESOURCE_NOTIFICATION_PROMO_H_
#define CHROME_BROWSER_WEB_RESOURCE_NOTIFICATION_PROMO_H_

#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "url/gurl.h"

class PrefRegistrySimple;
class PrefService;

namespace base {
class DictionaryValue;
class ListValue;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

class NotificationPromo {
 public:
  static GURL PromoServerURL();

  enum PromoType {
    NO_PROMO,
    NTP_NOTIFICATION_PROMO,
    NTP_BUBBLE_PROMO,
    MOBILE_NTP_SYNC_PROMO,
  };

  NotificationPromo();
  ~NotificationPromo();

  
  void InitFromJson(const base::DictionaryValue& json, PromoType promo_type);
  void InitFromPrefs(PromoType promo_type);

  
  bool CanShow() const;

  
  
  double StartTimeForGroup() const;
  double EndTime() const;

  
  
  static void HandleClosed(PromoType promo_type);
  
  
  static bool HandleViewed(PromoType promo_type);

  bool new_notification() const { return new_notification_; }

  const std::string& promo_text() const { return promo_text_; }
  PromoType promo_type() const { return promo_type_; }
  const base::DictionaryValue* promo_payload() const {
    return promo_payload_.get();
  }

  
  static void RegisterPrefs(PrefRegistrySimple* registry);
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);
  static void MigrateUserPrefs(PrefService* user_prefs);

 private:
  
  friend class NotificationPromoTest;

  
  
  void CheckForNewNotification();

  
  void OnNewNotification();

  
  void WritePrefs();

  
  
  bool ExceedsMaxGroup() const;

  
  
  bool ExceedsMaxViews() const;

  
  
  bool CheckAppLauncher() const;

  PrefService* prefs_;

  PromoType promo_type_;
  std::string promo_text_;

  scoped_ptr<const base::DictionaryValue> promo_payload_;

  double start_;
  double end_;

  int num_groups_;
  int initial_segment_;
  int increment_;
  int time_slice_;
  int max_group_;

  
  int max_views_;

  int group_;
  int views_;
  bool closed_;

  bool new_notification_;

  DISALLOW_COPY_AND_ASSIGN(NotificationPromo);
};

#endif  

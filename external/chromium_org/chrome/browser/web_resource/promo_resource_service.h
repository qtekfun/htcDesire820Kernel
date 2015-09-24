// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_WEB_RESOURCE_PROMO_RESOURCE_SERVICE_H_
#define CHROME_BROWSER_WEB_RESOURCE_PROMO_RESOURCE_SERVICE_H_

#include <string>

#include "base/memory/weak_ptr.h"
#include "chrome/browser/web_resource/web_resource_service.h"

class NotificationPromo;
class PrefRegistrySimple;
class PrefService;

namespace base {
class DictionaryValue;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

class PromoResourceService : public WebResourceService {
 public:
  static void RegisterPrefs(PrefRegistrySimple* registry);
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);
  static void MigrateUserPrefs(PrefService* user_prefs);

  PromoResourceService();

 private:
  virtual ~PromoResourceService();

  
  
  void ScheduleNotification(const NotificationPromo& notification_promo);

  
  
  
  
  void ScheduleNotificationOnInit();

  
  
  
  void PostNotification(int64 delay_ms);

  
  void PromoResourceStateChange();

  
  virtual void Unpack(const base::DictionaryValue& parsed_json) OVERRIDE;

  
  
  
  base::WeakPtrFactory<PromoResourceService> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(PromoResourceService);
};

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_WEB_RESOURCE_PROMO_RESOURCE_SERVICE_H_
#define CHROME_BROWSER_WEB_RESOURCE_PROMO_RESOURCE_SERVICE_H_
#pragma once

#include "chrome/browser/web_resource/web_resource_service.h"

namespace PromoResourceServiceUtil {

bool CanShowPromo(Profile* profile);

}  

class PrefService;

class PromoResourceService
    : public WebResourceService {
 public:
  static bool IsBuildTargeted(const std::string& channel, int builds_targeted);

  static void RegisterPrefs(PrefService* local_state);

  static void RegisterUserPrefs(PrefService* prefs);

  explicit PromoResourceService(Profile* profile);

  
  static const char* kDefaultPromoResourceServer;

 private:
  FRIEND_TEST_ALL_PREFIXES(PromoResourceServiceTest, UnpackLogoSignal);
  FRIEND_TEST_ALL_PREFIXES(PromoResourceServiceTest, UnpackPromoSignal);
  FRIEND_TEST_ALL_PREFIXES(PromoResourceServiceTest, UnpackWebStoreSignal);

  
  enum BuildType {
    NO_BUILD = 0,
    DEV_BUILD = 1,
    BETA_BUILD = 1 << 1,
    STABLE_BUILD = 1 << 2,
    CANARY_BUILD = 1 << 3,
  };

  virtual ~PromoResourceService();

  int GetPromoServiceVersion();

  
  
  std::string GetPromoLocale();

  void Init();

  
  
  
  bool IsThisBuildTargeted(int builds_targeted);

  
  
  void ScheduleNotification(double ms_start_time, double ms_end_time);

  
  
  
  
  void ScheduleNotificationOnInit();

  
  void set_channel(const char* channel) { channel_ = channel; }

  virtual void Unpack(const DictionaryValue& parsed_json);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void UnpackPromoSignal(const DictionaryValue& parsed_json);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void UnpackLogoSignal(const DictionaryValue& parsed_json);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void UnpackWebStoreSignal(const DictionaryValue& parsed_json);

  
  
  DictionaryValue* web_resource_cache_;

  
  const char* channel_;

  DISALLOW_COPY_AND_ASSIGN(PromoResourceService);
};

#endif  

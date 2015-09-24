// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NTP_ANDROID_PROMO_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_NTP_ANDROID_PROMO_HANDLER_H_

#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_ui_message_handler.h"

namespace base {
class DictionaryValue;
class ListValue;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

class PromoHandler : public content::WebUIMessageHandler,
                     public content::NotificationObserver {
 public:
  PromoHandler();
  virtual ~PromoHandler();

  
  virtual void RegisterMessages() OVERRIDE;

  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

 private:
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  void HandlePromoSendEmail(const base::ListValue* args);

  
  
  void HandlePromoActionTriggered(const base::ListValue* args);

  
  
  void HandlePromoDisabled(const base::ListValue* args);

  
  
  void HandleGetPromotions(const base::ListValue* args);

  
  
  void HandleRecordImpression(const base::ListValue* args);

  
  void InjectPromoDecorations();

  
  void RecordPromotionImpression(const std::string& id);

  
  
  bool FetchPromotion(base::DictionaryValue* result);

  
  bool DoesChromePromoMatchCurrentSync(
      bool promo_requires_sync,
      bool promo_requires_no_active_desktop_sync_sessions);

  
  void CheckDesktopSessions();

  
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(PromoHandler);
};

#endif  

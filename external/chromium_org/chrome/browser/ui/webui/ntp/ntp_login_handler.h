// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NTP_NTP_LOGIN_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_NTP_NTP_LOGIN_HANDLER_H_

#include "base/prefs/pref_member.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_ui_message_handler.h"

class Profile;

class NTPLoginHandler : public content::WebUIMessageHandler,
                        public content::NotificationObserver {
 public:
  NTPLoginHandler();
  virtual ~NTPLoginHandler();

  
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  static bool ShouldShow(Profile* profile);

  
  static void GetLocalizedValues(Profile* profile, DictionaryValue* values);

 private:
  
  enum NTPSignInPromoBuckets {
    NTP_SIGN_IN_PROMO_VIEWED,
    NTP_SIGN_IN_PROMO_CLICKED,
    NTP_SIGN_IN_PROMO_BUCKET_BOUNDARY,
  };

  
  
  void HandleInitializeSyncLogin(const ListValue* args);

  
  
  
  void HandleShowSyncLoginUI(const ListValue* args);

  
  void RecordInHistogram(int type);

  
  
  void HandleLoginMessageSeen(const ListValue* args);

  
  
  
  void HandleShowAdvancedLoginUI(const ListValue* args);

  
  void UpdateLogin();

  StringPrefMember username_pref_;
  BooleanPrefMember signin_allowed_pref_;
  content::NotificationRegistrar registrar_;
};

#endif  

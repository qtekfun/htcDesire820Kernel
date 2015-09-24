// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NTP_LOGIN_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_NTP_LOGIN_HANDLER_H_
#pragma once

#include "chrome/browser/prefs/pref_member.h"
#include "content/browser/webui/web_ui.h"
#include "content/common/notification_observer.h"

class NTPLoginHandler : public WebUIMessageHandler,
                        public NotificationObserver {
 public:
  NTPLoginHandler();
  ~NTPLoginHandler();

  virtual WebUIMessageHandler* Attach(WebUI* web_ui);

  
  virtual void RegisterMessages();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  
  void HandleInitializeLogin(const ListValue* args);

  
  void UpdateLogin();

  StringPrefMember username_pref_;
};

#endif  

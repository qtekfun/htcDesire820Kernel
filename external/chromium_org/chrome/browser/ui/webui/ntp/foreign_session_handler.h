// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NTP_FOREIGN_SESSION_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_NTP_FOREIGN_SESSION_HANDLER_H_

#include <vector>

#include "base/time/time.h"
#include "chrome/browser/sessions/session_service.h"
#include "chrome/browser/sync/open_tabs_ui_delegate.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_ui.h"
#include "content/public/browser/web_ui_message_handler.h"

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace browser_sync {

class ForeignSessionHandler : public content::WebUIMessageHandler,
                              public content::NotificationObserver {
 public:
  
  static const int kInvalidId = -1;

  
  virtual void RegisterMessages() OVERRIDE;

  ForeignSessionHandler();
  virtual ~ForeignSessionHandler() {}

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  static void OpenForeignSessionTab(content::WebUI* web_ui,
                                    const std::string& session_string_value,
                                    SessionID::id_type window_num,
                                    SessionID::id_type tab_id,
                                    const WindowOpenDisposition& disposition);

  static void OpenForeignSessionWindows(content::WebUI* web_ui,
                                        const std::string& session_string_value,
                                        SessionID::id_type window_num);

  
  static bool SessionTabToValue(const SessionTab& tab,
                                DictionaryValue* dictionary);

  
  static OpenTabsUIDelegate* GetOpenTabsUIDelegate(content::WebUI* web_ui);

 private:
  
  void Init();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  bool IsTabSyncEnabled();

  
  base::string16 FormatSessionTime(const base::Time& time);

  
  
  
  void HandleOpenForeignSession(const ListValue* args);

  
  
  
  void HandleGetForeignSessions(const ListValue* args);

  
  
  
  
  void HandleDeleteForeignSession(const ListValue* args);

  void HandleSetForeignSessionCollapsed(const ListValue* args);

  
  bool SessionWindowToValue(const SessionWindow& window,
                            DictionaryValue* dictionary);

  
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(ForeignSessionHandler);
};

}  

#endif  

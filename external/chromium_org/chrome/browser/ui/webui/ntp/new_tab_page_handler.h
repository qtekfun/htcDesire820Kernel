// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NTP_NEW_TAB_PAGE_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_NTP_NEW_TAB_PAGE_HANDLER_H_

#include "base/memory/weak_ptr.h"
#include "base/values.h"
#include "chrome/browser/chrome_notification_types.h"
#include "content/public/browser/web_ui_message_handler.h"

class PrefRegistrySimple;
class Profile;

namespace user_prefs {
class PrefRegistrySyncable;
}

class NewTabPageHandler : public content::WebUIMessageHandler,
                          public base::SupportsWeakPtr<NewTabPageHandler> {
 public:
  NewTabPageHandler();

  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  static void GetLocalizedValues(Profile* profile, DictionaryValue* values);

 private:
  virtual ~NewTabPageHandler();

  
  virtual void RegisterMessages() OVERRIDE;

  
  void HandleNotificationPromoClosed(const ListValue* args);

  
  void HandleNotificationPromoViewed(const ListValue* args);

  
  void HandleNotificationPromoLinkClicked(const ListValue* args);

  
  void HandleBubblePromoClosed(const ListValue* args);

  
  void HandleBubblePromoViewed(const ListValue* args);

  
  void HandleBubblePromoLinkClicked(const ListValue* args);

  
  void HandlePageSelected(const ListValue* args);

  
  void HandleLogTimeToClick(const base::ListValue* args);

  
  size_t page_switch_count_;

  
  
  
  
  static const int kPageIdOffset = 10;
  enum {
    INDEX_MASK = (1 << kPageIdOffset) - 1,
    MOST_VISITED_PAGE_ID = 1 << kPageIdOffset,
    APPS_PAGE_ID = 2 << kPageIdOffset,
    BOOKMARKS_PAGE_ID = 3 << kPageIdOffset,
    SUGGESTIONS_PAGE_ID = 4 << kPageIdOffset,
    LAST_PAGE_ID = SUGGESTIONS_PAGE_ID
  };
  static const int kHistogramEnumerationMax =
      (LAST_PAGE_ID >> kPageIdOffset) + 1;

  
  void Notify(chrome::NotificationType notification_type);

  DISALLOW_COPY_AND_ASSIGN(NewTabPageHandler);
};

#endif  

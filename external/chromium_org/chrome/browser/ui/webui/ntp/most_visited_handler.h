// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NTP_MOST_VISITED_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_NTP_MOST_VISITED_HANDLER_H_

#include <string>
#include <vector>

#include "base/memory/weak_ptr.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/browser/history/history_types.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_ui_message_handler.h"

class GURL;
class PageUsageData;

namespace base {
class ListValue;
class Value;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

class MostVisitedHandler : public content::WebUIMessageHandler,
                           public content::NotificationObserver {
 public:

  MostVisitedHandler();
  virtual ~MostVisitedHandler();

  
  virtual void RegisterMessages() OVERRIDE;

  
  void HandleGetMostVisited(const base::ListValue* args);

  
  void HandleBlacklistUrl(const base::ListValue* args);

  
  void HandleRemoveUrlsFromBlacklist(const base::ListValue* args);

  
  void HandleClearBlacklist(const base::ListValue* args);

  
  void HandleMostVisitedAction(const base::ListValue* args);

  
  void HandleMostVisitedSelected(const base::ListValue* args);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  const std::vector<GURL>& most_visited_urls() const {
    return most_visited_urls_;
  }

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

 private:
  struct MostVisitedPage;

  
  void StartQueryForMostVisited();

  
  void SetPagesValueFromTopSites(const history::MostVisitedURLList& data);

  
  void OnMostVisitedUrlsAvailable(const history::MostVisitedURLList& data);

  
  void BlacklistUrl(const GURL& url);

  
  std::string GetDictionaryKeyForUrl(const std::string& url);

  
  
  
  void MaybeRemovePageValues();

  
  void SendPagesValue();

  content::NotificationRegistrar registrar_;

  
  
  
  std::vector<GURL> most_visited_urls_;

  
  
  bool got_first_most_visited_request_;

  
  scoped_ptr<base::ListValue> pages_value_;

  
  bool most_visited_viewed_;

  
  bool user_action_logged_;

  
  base::WeakPtrFactory<MostVisitedHandler> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(MostVisitedHandler);
};

#endif  

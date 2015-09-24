// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_MOST_VISITED_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_MOST_VISITED_HANDLER_H_
#pragma once

#include <string>
#include <vector>

#include "chrome/browser/history/history_types.h"
#include "content/browser/cancelable_request.h"
#include "content/browser/webui/web_ui.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class GURL;
class ListValue;
class PageUsageData;
class PrefService;
class Value;

class MostVisitedHandler : public WebUIMessageHandler,
                           public NotificationObserver {
 public:

  MostVisitedHandler();
  virtual ~MostVisitedHandler();

  
  virtual WebUIMessageHandler* Attach(WebUI* web_ui);
  virtual void RegisterMessages();

  
  void HandleGetMostVisited(const ListValue* args);

  
  void HandleBlacklistURL(const ListValue* args);

  
  void HandleRemoveURLsFromBlacklist(const ListValue* args);

  
  void HandleClearBlacklist(const ListValue* args);

  
  void HandleAddPinnedURL(const ListValue* args);

  
  void HandleRemovePinnedURL(const ListValue* args);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  const std::vector<GURL>& most_visited_urls() const {
    return most_visited_urls_;
  }

  static void RegisterUserPrefs(PrefService* prefs);

  
  
  static std::vector<GURL> GetPrePopulatedUrls();

 private:
  struct MostVisitedPage;

  
  void StartQueryForMostVisited();

  
  void SetPagesValueFromTopSites(const history::MostVisitedURLList& data);

  
  void OnMostVisitedURLsAvailable(const history::MostVisitedURLList& data);

  
  void BlacklistURL(const GURL& url);

  
  
  std::string GetDictionaryKeyForURL(const std::string& url);

  
  
  bool GetPinnedURLAtIndex(int index, MostVisitedPage* page);

  void AddPinnedURL(const MostVisitedPage& page, int index);
  void RemovePinnedURL(const GURL& url);

  
  void SendPagesValue();

  
  bool IsFirstRun();

  static const std::vector<MostVisitedPage>& GetPrePopulatedPages();

  NotificationRegistrar registrar_;

  
  CancelableRequestConsumerTSimple<PageUsageData*> cancelable_consumer_;
  CancelableRequestConsumer topsites_consumer_;

  
  
  
  std::vector<GURL> most_visited_urls_;

  
  
  bool got_first_most_visited_request_;

  
  scoped_ptr<ListValue> pages_value_;

  DISALLOW_COPY_AND_ASSIGN(MostVisitedHandler);
};

#endif  

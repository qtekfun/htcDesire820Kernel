// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NTP_SUGGESTIONS_PAGE_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_NTP_SUGGESTIONS_PAGE_HANDLER_H_

#include <string>

#include "chrome/browser/common/cancelable_request.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/ui/webui/ntp/suggestions_combiner.h"
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

class SuggestionsHandler : public content::WebUIMessageHandler,
                           public content::NotificationObserver,
                           public SuggestionsCombiner::Delegate {
 public:
  SuggestionsHandler();
  virtual ~SuggestionsHandler();

  
  virtual void RegisterMessages() OVERRIDE;

  
  void HandleGetSuggestions(const base::ListValue* args);

  
  void HandleBlacklistURL(const base::ListValue* args);

  
  void HandleRemoveURLsFromBlacklist(const base::ListValue* args);

  
  void HandleClearBlacklist(const base::ListValue* args);

  
  void HandleSuggestedSitesAction(const base::ListValue* args);

  
  void HandleSuggestedSitesSelected(const base::ListValue* args);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void OnSuggestionsReady() OVERRIDE;

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

 private:
  
  void BlacklistURL(const GURL& url);

  
  std::string GetDictionaryKeyForURL(const std::string& url);

  
  void SendPagesValue();

  content::NotificationRegistrar registrar_;

  
  
  bool got_first_suggestions_request_;

  
  scoped_ptr<SuggestionsCombiner> suggestions_combiner_;

  
  bool suggestions_viewed_;

  
  bool user_action_logged_;

  DISALLOW_COPY_AND_ASSIGN(SuggestionsHandler);
};

#endif  

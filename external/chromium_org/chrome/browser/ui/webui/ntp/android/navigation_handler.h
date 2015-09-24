// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NTP_ANDROID_NAVIGATION_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_NTP_ANDROID_NAVIGATION_HANDLER_H_

#include "base/compiler_specific.h"
#include "content/public/browser/navigation_entry.h"
#include "content/public/browser/web_ui_message_handler.h"

namespace base {
class ListValue;
}

class NavigationHandler : public content::WebUIMessageHandler {
 public:
  NavigationHandler();
  virtual ~NavigationHandler();

  
  virtual void RegisterMessages() OVERRIDE;

  
  void HandleOpenedMostVisited(const base::ListValue* args);

  
  void HandleOpenedRecentlyClosed(const base::ListValue* args);

  
  void HandleOpenedBookmark(const base::ListValue* args);

  
  void HandleOpenedForeignSession(const base::ListValue* args);

  static void RecordActionForNavigation(const content::NavigationEntry& entry);

 private:
  
  
  
  enum Action {
    
    ACTION_SEARCHED_USING_OMNIBOX = 0,
    
    ACTION_NAVIGATED_TO_GOOGLE_HOMEPAGE = 1,
    
    ACTION_NAVIGATED_USING_OMNIBOX = 2,
    
    ACTION_OPENED_MOST_VISITED_ENTRY = 3,
    
    ACTION_OPENED_RECENTLY_CLOSED_ENTRY = 4,
    
    ACTION_OPENED_BOOKMARK = 5,
    
    ACTION_OPENED_FOREIGN_SESSION = 6,
    
    NUM_ACTIONS = 7
  };

  static void RecordAction(Action action);

  DISALLOW_COPY_AND_ASSIGN(NavigationHandler);
};

#endif  

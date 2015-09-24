// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BOOKMARKS_BOOKMARK_PROMPT_CONTROLLER_H_
#define CHROME_BROWSER_UI_BOOKMARKS_BOOKMARK_PROMPT_CONTROLLER_H_

#include "base/basictypes.h"
#include "base/time/time.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/ui/browser_list_observer.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "url/gurl.h"

class Browser;
class PrefService;

namespace content {
class WebContents;
}

class BookmarkPromptController : public chrome::BrowserListObserver,
                                 public content::NotificationObserver,
                                 public TabStripModelObserver {
 public:
  
  
  static const int kMaxPromptImpressionCount;

  
  static const int kVisitCountForPermanentTrigger;

  
  
  static const int kVisitCountForSessionTrigger;

  
  
  BookmarkPromptController();
  virtual ~BookmarkPromptController();

  
  
  
  static void AddedBookmark(Browser* browser, const GURL& url);

  
  static void ClosingBookmarkPrompt();

  
  static void DisableBookmarkPrompt(PrefService* prefs);

  
  static bool IsEnabled();

 private:
  
  virtual void ActiveTabChanged(content::WebContents* old_contents,
                                content::WebContents* new_contents,
                                int index,
                                int reason) OVERRIDE;

  void AddedBookmarkInternal(Browser* browser, const GURL& url);
  void ClosingBookmarkPromptInternal();

   
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void OnBrowserRemoved(Browser* browser) OVERRIDE;
  virtual void OnBrowserSetLastActive(Browser* browser) OVERRIDE;

  
  void OnDidQueryURL(CancelableRequestProvider::Handle handle,
                     bool success,
                     const history::URLRow* url_row,
                     history::VisitVector* visits);

  
  void SetBrowser(Browser* browser);

  
  
  void SetWebContents(content::WebContents* web_contents);

  
  Browser* browser_;

  
  content::WebContents* web_contents_;

  
  GURL last_prompted_url_;

  
  
  base::Time last_prompted_time_;

  
  base::Time query_url_start_time_;

  CancelableRequestConsumer query_url_consumer_;
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkPromptController);
};

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_HISTORY_TAB_HELPER_H_
#define CHROME_BROWSER_HISTORY_HISTORY_TAB_HELPER_H_

#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

class HistoryService;

namespace history {
struct HistoryAddPageArgs;
}

class HistoryTabHelper : public content::WebContentsObserver,
                         public content::WebContentsUserData<HistoryTabHelper> {
 public:
  virtual ~HistoryTabHelper();

  
  
  void UpdateHistoryForNavigation(
      const history::HistoryAddPageArgs& add_page_args);

  
  
  void UpdateHistoryPageTitle(const content::NavigationEntry& entry);

  
  
  history::HistoryAddPageArgs CreateHistoryAddPageArgs(
      const GURL& virtual_url,
      base::Time timestamp,
      bool did_replace_entry,
      const content::FrameNavigateParams& params);

 private:
  explicit HistoryTabHelper(content::WebContents* web_contents);
  friend class content::WebContentsUserData<HistoryTabHelper>;

  
  virtual void DidNavigateMainFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) OVERRIDE;
  virtual void DidNavigateAnyFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) OVERRIDE;
  virtual void TitleWasSet(content::NavigationEntry* entry,
                           bool explicit_set) OVERRIDE;
  virtual void WebContentsDestroyed(content::WebContents* tab) OVERRIDE;

  
  HistoryService* GetHistoryService();

  
  
  
  
  bool received_page_title_;

  DISALLOW_COPY_AND_ASSIGN(HistoryTabHelper);
};

#endif  

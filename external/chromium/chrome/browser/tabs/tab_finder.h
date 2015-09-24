// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TABS_TAB_FINDER_H_
#define CHROME_BROWSER_TABS_TAB_FINDER_H_
#pragma once

#include <map>
#include <set>

#include "base/basictypes.h"
#include "base/memory/singleton.h"
#include "chrome/browser/history/history_types.h"
#include "content/browser/cancelable_request.h"
#include "content/browser/tab_contents/navigation_controller.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class Browser;
class GURL;
class TabContents;

class TabFinder : public NotificationObserver {
 public:
  
  static TabFinder* GetInstance();

  
  static bool IsEnabled();

  
  
  
  TabContents* FindTab(Browser* browser,
                       const GURL& url,
                       Browser** existing_browser);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

 private:
  friend struct DefaultSingletonTraits<TabFinder>;

  class TabContentsObserverImpl;

  typedef std::map<TabContents*, GURL> TabContentsToURLMap;
  typedef std::set<TabContentsObserverImpl*> TabContentsObservers;

  TabFinder();
  ~TabFinder();

  void Init();

  
  void DidNavigateAnyFramePostCommit(
      TabContents* source,
      const NavigationController::LoadCommittedDetails& details,
      const ViewHostMsg_FrameNavigate_Params& params);

  
  
  bool TabMatchesURL(TabContents* tab_contents, const GURL& url);

  
  
  TabContents* FindTabInBrowser(Browser* browser, const GURL& url);

  
  
  void TrackTab(TabContents* tab);

  
  void TrackBrowser(Browser* browser);

  
  void TabDestroyed(TabContentsObserverImpl* observer);

  
  void CancelRequestsFor(TabContents* tab_contents);

  
  
  void FetchRedirectStart(TabContents* tab);

  
  void QueryRedirectsToComplete(CancelableRequestProvider::Handle handle,
                                GURL url,
                                bool success,
                                history::RedirectList* redirects);

  
  TabContentsToURLMap tab_contents_to_url_;

  CancelableRequestConsumerTSimple<TabContents*> callback_consumer_;

  NotificationRegistrar registrar_;

  TabContentsObservers tab_contents_observers_;

  DISALLOW_COPY_AND_ASSIGN(TabFinder);
};

#endif  

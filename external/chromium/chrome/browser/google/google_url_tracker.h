// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_GOOGLE_GOOGLE_URL_TRACKER_H_
#define CHROME_BROWSER_GOOGLE_GOOGLE_URL_TRACKER_H_
#pragma once

#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/tab_contents/confirm_infobar_delegate.h"
#include "chrome/common/net/url_fetcher.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "googleurl/src/gurl.h"
#include "net/base/network_change_notifier.h"

class NavigationController;
class PrefService;
class TabContents;
class TemplateURL;

class GoogleURLTracker : public URLFetcher::Delegate,
                         public NotificationObserver,
                         public net::NetworkChangeNotifier::IPAddressObserver {
 public:
  
  
  
  
  
  
  GoogleURLTracker();

  virtual ~GoogleURLTracker();

  
  
  
  
  static GURL GoogleURL();

  
  
  
  
  
  
  
  static void RequestServerCheck();

  static void RegisterPrefs(PrefService* prefs);

  
  
  
  
  static void GoogleURLSearchCommitted();

  static const char kDefaultGoogleHomepage[];
  static const char kSearchDomainCheckURL[];

  
  void AcceptGoogleURL(const GURL& google_url);
  void CancelGoogleURL(const GURL& google_url);
  void InfoBarClosed();
  void RedoSearch();

 private:
  friend class GoogleURLTrackerTest;

  typedef InfoBarDelegate* (*InfobarCreator)(TabContents*,
                                             GoogleURLTracker*,
                                             const GURL&);

  
  
  
  void SetNeedToFetch();

  
  
  void QueueWakeupTask();

  
  
  void FinishSleep();

  
  
  void StartFetchIfDesirable();

  
  virtual void OnURLFetchComplete(const URLFetcher *source,
                                  const GURL& url,
                                  const net::URLRequestStatus& status,
                                  int response_code,
                                  const ResponseCookies& cookies,
                                  const std::string& data);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual void OnIPAddressChanged();

  void SearchCommitted();
  void OnNavigationPending(const NotificationSource& source,
                           const GURL& pending_url);
  void OnNavigationCommittedOrTabClosed(TabContents* tab_contents,
                                        NotificationType::Type type);
  void ShowGoogleURLInfoBarIfNecessary(TabContents* tab_contents);

  NotificationRegistrar registrar_;
  InfobarCreator infobar_creator_;
  
  
  
  GURL google_url_;
  GURL fetched_google_url_;
  ScopedRunnableMethodFactory<GoogleURLTracker> runnable_method_factory_;
  scoped_ptr<URLFetcher> fetcher_;
  int fetcher_id_;
  bool queue_wakeup_task_;
  bool in_startup_sleep_;  
                           
  bool already_fetched_;   
                           
  bool need_to_fetch_;     
                           
                           
                           
                           
  bool need_to_prompt_;    
                           
                           
  NavigationController* controller_;
  InfoBarDelegate* infobar_;
  GURL search_url_;

  DISALLOW_COPY_AND_ASSIGN(GoogleURLTracker);
};


class GoogleURLTrackerInfoBarDelegate : public ConfirmInfoBarDelegate {
 public:
  GoogleURLTrackerInfoBarDelegate(TabContents* tab_contents,
                                  GoogleURLTracker* google_url_tracker,
                                  const GURL& new_google_url);

  
  virtual bool Accept();
  virtual bool Cancel();
  virtual void InfoBarClosed();

 protected:
  virtual ~GoogleURLTrackerInfoBarDelegate();

  GoogleURLTracker* google_url_tracker_;
  const GURL new_google_url_;

 private:
  
  virtual string16 GetMessageText() const;
  virtual string16 GetButtonLabel(InfoBarButton button) const;

  DISALLOW_COPY_AND_ASSIGN(GoogleURLTrackerInfoBarDelegate);
};

#endif  

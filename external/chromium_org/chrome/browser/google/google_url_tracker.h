// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_GOOGLE_GOOGLE_URL_TRACKER_H_
#define CHROME_BROWSER_GOOGLE_GOOGLE_URL_TRACKER_H_

#include <map>
#include <string>
#include <utility>

#include "base/callback_forward.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/google/google_url_tracker_map_entry.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "net/base/network_change_notifier.h"
#include "net/url_request/url_fetcher.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "url/gurl.h"

class GoogleURLTrackerNavigationHelper;
class InfoBar;
class PrefService;
class Profile;

namespace content {
class NavigationController;
}

class GoogleURLTracker : public net::URLFetcherDelegate,
                         public net::NetworkChangeNotifier::IPAddressObserver,
                         public BrowserContextKeyedService {
 public:
  
  typedef std::pair<GURL, GURL> UpdatedDetails;

  
  
  enum Mode {
    NORMAL_MODE,
    UNIT_TEST_MODE,
  };

  
  
  
  GoogleURLTracker(Profile* profile,
                   scoped_ptr<GoogleURLTrackerNavigationHelper> nav_helper,
                   Mode mode);

  virtual ~GoogleURLTracker();

  
  
  
  
  static GURL GoogleURL(Profile* profile);

  
  
  
  
  
  
  
  
  
  static void RequestServerCheck(Profile* profile, bool force);

  
  
  
  
  
  
  static void GoogleURLSearchCommitted(Profile* profile);

  
  void AcceptGoogleURL(bool redo_searches);
  void CancelGoogleURL();
  const GURL& google_url() const { return google_url_; }
  const GURL& fetched_google_url() const { return fetched_google_url_; }

  
  void DeleteMapEntryForService(const InfoBarService* infobar_service);

  
  
  
  
  
  
  
  
  virtual void OnNavigationPending(
      content::NavigationController* navigation_controller,
      InfoBarService* infobar_service,
      int pending_id);

  
  
  
  virtual void OnNavigationCommitted(InfoBarService* infobar_service,
                                     const GURL& search_url);

  
  virtual void OnTabClosed(
      content::NavigationController* navigation_controller);

  static const char kDefaultGoogleHomepage[];
  static const char kSearchDomainCheckURL[];

 private:
  friend class GoogleURLTrackerTest;

  typedef std::map<const InfoBarService*, GoogleURLTrackerMapEntry*> EntryMap;

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  virtual void OnIPAddressChanged() OVERRIDE;

  
  virtual void Shutdown() OVERRIDE;

  
  
  
  void SetNeedToFetch();

  
  
  void FinishSleep();

  
  
  void StartFetchIfDesirable();

  
  
  
  void SearchCommitted();

  
  
  
  void CloseAllEntries(bool redo_searches);

  
  
  
  
  
  
  void UnregisterForEntrySpecificNotifications(
      const GoogleURLTrackerMapEntry& map_entry,
      bool must_be_listening_for_commit);

  Profile* profile_;

  scoped_ptr<GoogleURLTrackerNavigationHelper> nav_helper_;

  
  
  
  base::Callback<InfoBar*(InfoBarService*, GoogleURLTracker*, const GURL&)>
      infobar_creator_;

  GURL google_url_;
  GURL fetched_google_url_;
  base::WeakPtrFactory<GoogleURLTracker> weak_ptr_factory_;
  scoped_ptr<net::URLFetcher> fetcher_;
  int fetcher_id_;
  bool in_startup_sleep_;  
                           
  bool already_fetched_;   
                           
  bool need_to_fetch_;     
                           
                           
                           
                           
  bool need_to_prompt_;    
                           
                           
  bool search_committed_;  
                           
  EntryMap entry_map_;

  DISALLOW_COPY_AND_ASSIGN(GoogleURLTracker);
};

#endif  

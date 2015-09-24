// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_INSTANT_SERVICE_H_
#define CHROME_BROWSER_SEARCH_INSTANT_SERVICE_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/prefs/pref_change_registrar.h"
#include "chrome/browser/google/google_url_tracker.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/ui/search/instant_ntp_prerenderer.h"
#include "chrome/browser/ui/search/instant_search_prerenderer.h"
#include "chrome/common/instant_types.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class GURL;
class InstantIOContext;
class InstantServiceObserver;
class InstantTestBase;
class InstantServiceTest;
class Profile;
class ThemeService;

namespace content {
class WebContents;
}

namespace net {
class URLRequest;
}

class InstantService : public BrowserContextKeyedService,
                       public content::NotificationObserver {
 public:
  explicit InstantService(Profile* profile);
  virtual ~InstantService();

  
  
  void AddInstantProcess(int process_id);
  bool IsInstantProcess(int process_id) const;

  
  void AddObserver(InstantServiceObserver* observer);
  void RemoveObserver(InstantServiceObserver* observer);

#if defined(UNIT_TEST)
  int GetInstantProcessCount() const {
    return process_ids_.size();
  }
#endif

  

  
  
  void DeleteMostVisitedItem(const GURL& url);

  
  
  void UndoMostVisitedDeletion(const GURL& url);

  
  
  void UndoAllMostVisitedDeletions();

  
  
  
  
  void UpdateThemeInfo();

  
  
  void UpdateMostVisitedItemsInfo();

  
  
  
  scoped_ptr<content::WebContents> ReleaseNTPContents() WARN_UNUSED_RESULT;

  
  content::WebContents* GetNTPContents() const;

  
  
  void OnBrowserInstantControllerCreated();

  
  
  void OnBrowserInstantControllerDestroyed();

  
  void SendSearchURLsToRenderer(content::RenderProcessHost* rph);

  InstantSearchPrerenderer* instant_search_prerenderer() {
    return instant_prerenderer_.get();
  }

 private:
  friend class InstantExtendedTest;
  friend class InstantServiceTest;
  friend class InstantTestBase;
  friend class InstantUnitTestBase;

  FRIEND_TEST_ALL_PREFIXES(InstantExtendedNetworkTest,
                           NTPReactsToNetworkChanges);
  FRIEND_TEST_ALL_PREFIXES(InstantExtendedManualTest,
                           MANUAL_ShowsGoogleNTP);
  FRIEND_TEST_ALL_PREFIXES(InstantExtendedManualTest,
                           MANUAL_SearchesFromFakebox);
  FRIEND_TEST_ALL_PREFIXES(InstantExtendedTest, ProcessIsolation);
  FRIEND_TEST_ALL_PREFIXES(InstantServiceTest, SendsSearchURLsToRenderer);

  
  virtual void Shutdown() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void OnRendererProcessTerminated(int process_id);

  
  
  
  void OnMostVisitedItemsReceived(const history::MostVisitedURLList& data);

  
  void NotifyAboutMostVisitedItems();

  
  void OnThemeChanged(ThemeService* theme_service);

  void OnGoogleURLUpdated(Profile* profile,
                          GoogleURLTracker::UpdatedDetails* details);

  void OnDefaultSearchProviderChanged(const std::string& pref_name);

  
  InstantNTPPrerenderer* ntp_prerenderer();

  void ResetInstantSearchPrerenderer();

  Profile* const profile_;

  
  std::set<int> process_ids_;

  
  std::vector<InstantMostVisitedItem> most_visited_items_;

  
  scoped_ptr<ThemeBackgroundInfo> theme_info_;

  ObserverList<InstantServiceObserver> observers_;

  content::NotificationRegistrar registrar_;

  PrefChangeRegistrar profile_pref_registrar_;

  scoped_refptr<InstantIOContext> instant_io_context_;

  InstantNTPPrerenderer ntp_prerenderer_;

  
  
  size_t browser_instant_controller_object_count_;

  
  scoped_ptr<InstantSearchPrerenderer> instant_prerenderer_;

  
  base::WeakPtrFactory<InstantService> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(InstantService);
};

#endif  

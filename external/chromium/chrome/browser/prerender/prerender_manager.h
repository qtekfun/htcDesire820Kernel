// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRERENDER_PRERENDER_MANAGER_H_
#define CHROME_BROWSER_PRERENDER_PRERENDER_MANAGER_H_
#pragma once

#include <list>
#include <map>
#include <vector>

#include "base/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time.h"
#include "base/timer.h"
#include "chrome/browser/prerender/prerender_contents.h"
#include "googleurl/src/gurl.h"

class Profile;
class TabContents;

#if defined(COMPILER_GCC)

namespace __gnu_cxx {
template <>
struct hash<TabContents*> {
  std::size_t operator()(TabContents* value) const {
    return reinterpret_cast<std::size_t>(value);
  }
};
}

#endif

namespace prerender {

class PrerenderManager : public base::RefCountedThreadSafe<PrerenderManager> {
 public:
  
  
  enum PrerenderManagerMode {
    PRERENDER_MODE_DISABLED,
    PRERENDER_MODE_ENABLED,
    PRERENDER_MODE_EXPERIMENT_CONTROL_GROUP,
    PRERENDER_MODE_EXPERIMENT_PRERENDER_GROUP,
    PRERENDER_MODE_MAX
  };

  
  explicit PrerenderManager(Profile* profile);

  
  
  
  bool AddPreload(const GURL& url, const std::vector<GURL>& alias_urls,
                  const GURL& referrer);

  void AddPendingPreload(const std::pair<int, int>& child_route_id_pair,
                         const GURL& url,
                         const std::vector<GURL>& alias_urls,
                         const GURL& referrer);

  
  
  
  
  bool MaybeUsePreloadedPage(TabContents* tc, const GURL& url);

  
  
  void RemoveEntry(PrerenderContents* entry);

  
  
  
  
  PrerenderContents* GetEntry(const GURL& url);

  
  
  
  
  static void RecordPerceivedPageLoadTime(
      base::TimeDelta perceived_page_load_time,
      TabContents* tab_contents);

  
  
  void RecordTimeUntilUsed(base::TimeDelta time_until_used);

  base::TimeDelta max_prerender_age() const { return max_prerender_age_; }
  void set_max_prerender_age(base::TimeDelta td) { max_prerender_age_ = td; }
  unsigned int max_elements() const { return max_elements_; }
  void set_max_elements(unsigned int num) { max_elements_ = num; }

  
  
  bool is_enabled() const;

  
  
  
  
  void set_enabled(bool enabled);

  static PrerenderManagerMode GetMode();
  static void SetMode(PrerenderManagerMode mode);
  static bool IsPrerenderingPossible();
  static bool IsControlGroup();

  
  
  static void RecordPrefetchTagObserved();

  
  
  void MarkTabContentsAsPrerendered(TabContents* tc);
  void MarkTabContentsAsWouldBePrerendered(TabContents* tc);
  void MarkTabContentsAsNotPrerendered(TabContents* tc);
  bool IsTabContentsPrerendered(TabContents* tc) const;
  bool WouldTabContentsBePrerendered(TabContents* tc) const;

  
  
  
  static bool MaybeGetQueryStringBasedAliasURL(const GURL& url,
                                               GURL* alias_url);

 protected:
  struct PendingContentsData;

  virtual ~PrerenderManager();

  void SetPrerenderContentsFactory(
      PrerenderContents::Factory* prerender_contents_factory);
  bool rate_limit_enabled_;

  PendingContentsData* FindPendingEntry(const GURL& url);

 private:
  
  friend class PrerenderBrowserTest;

  friend class base::RefCountedThreadSafe<PrerenderManager>;

  struct PrerenderContentsData;

  
  void StartSchedulingPeriodicCleanups();
  void StopSchedulingPeriodicCleanups();

  
  
  
  void PeriodicCleanup();

  bool IsPrerenderElementFresh(const base::Time start) const;
  void DeleteOldEntries();
  virtual base::Time GetCurrentTime() const;
  virtual base::TimeTicks GetCurrentTimeTicks() const;
  virtual PrerenderContents* CreatePrerenderContents(
      const GURL& url,
      const std::vector<GURL>& alias_urls,
      const GURL& referrer);

  
  
  
  PrerenderContents* FindEntry(const GURL& url);

  static bool WithinWindow();

  static void RecordPrefetchTagObservedOnUIThread();

  
  
  void RemovePendingPreload(PrerenderContents* entry);

  bool DoesRateLimitAllowPrerender() const;

  
  
  
  bool enabled_;

  Profile* profile_;

  base::TimeDelta max_prerender_age_;
  unsigned int max_elements_;

  
  std::list<PrerenderContentsData> prerender_list_;

  
  base::hash_set<TabContents*> prerendered_tc_set_;

  
  
  base::hash_set<TabContents*> would_be_prerendered_tc_set_;

  
  typedef std::map<std::pair<int, int>, std::vector<PendingContentsData> >
      PendingPrerenderList;
  PendingPrerenderList pending_prerender_list_;

  
  static const unsigned int kDefaultMaxPrerenderElements = 1;

  
  static const int kDefaultMaxPrerenderAgeSeconds = 20;

  
  
  static const int kWindowDurationSeconds = 30;

  
  static const int kPeriodicCleanupIntervalMs = 1000;

  
  static const int kMinTimeBetweenPrerendersMs = 500;

  scoped_ptr<PrerenderContents::Factory> prerender_contents_factory_;

  static PrerenderManagerMode mode_;

  
  
  
  
  static base::TimeTicks last_prefetch_seen_time_;

  
  
  static int prerenders_per_session_count_;

  
  
  base::RepeatingTimer<PrerenderManager> repeating_timer_;

  
  base::TimeTicks last_prerender_start_time_;

  DISALLOW_COPY_AND_ASSIGN(PrerenderManager);
};

}  

#endif  

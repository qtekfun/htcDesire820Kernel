// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRERENDER_PRERENDER_TRACKER_H_
#define CHROME_BROWSER_PRERENDER_PRERENDER_TRACKER_H_

#include <map>
#include <set>
#include <utility>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"
#include "base/synchronization/lock.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/prerender/prerender_contents.h"
#include "chrome/browser/prerender/prerender_final_status.h"
#include "url/gurl.h"

namespace prerender {

class PrerenderManager;
class PrerenderPendingSwapThrottle;
class PrerenderResourceThrottle;
struct RenderViewInfo;

class PrerenderTracker : public base::NonThreadSafe,
                         public PrerenderContents::Observer {
 public:
  typedef std::pair<int, int> ChildRouteIdPair;

  PrerenderTracker();
  virtual ~PrerenderTracker();

  
  
  
  
  
  
  
  
  bool TryUse(int child_id, int route_id);

  
  
  
  
  
  
  
  
  
  bool TryCancel(int child_id, int route_id, FinalStatus final_status);

  
  
  bool TryCancelOnIOThread(int child_id, int route_id,
                           FinalStatus final_status);

  
  
  
  bool GetFinalStatus(int child_id, int route_id,
                      FinalStatus* final_status) const;

  
  
  
  bool IsPrerenderingOnIOThread(int child_id, int route_id) const;

  
  
  
  bool IsPendingSwapRequestOnIOThread(int child_id, int route_id,
                                      const GURL& url) const;

  
  
  
  void AddResourceThrottleOnIOThread(
      int child_id, int route_id,
      const base::WeakPtr<PrerenderResourceThrottle>& throttle);

  
  
  
  void AddPendingSwapThrottleOnIOThread(
      int child_id, int route_id, const GURL& url,
      const base::WeakPtr<PrerenderPendingSwapThrottle>& throttle);

  
  void AddPrerenderPendingSwap(const ChildRouteIdPair& child_route_id_pair,
                               const GURL& url);

  
  void RemovePrerenderPendingSwap(const ChildRouteIdPair& child_route_id_pair,
                                  bool swap_successful);

 private:
  friend class PrerenderContents;
  FRIEND_TEST_ALL_PREFIXES(PrerenderTrackerTest, PrerenderTrackerNull);
  FRIEND_TEST_ALL_PREFIXES(PrerenderTrackerTest, PrerenderTrackerUsed);
  FRIEND_TEST_ALL_PREFIXES(PrerenderTrackerTest, PrerenderTrackerCancelled);
  FRIEND_TEST_ALL_PREFIXES(PrerenderTrackerTest, PrerenderTrackerCancelledOnIO);
  FRIEND_TEST_ALL_PREFIXES(PrerenderTrackerTest, PrerenderTrackerCancelledFast);
  FRIEND_TEST_ALL_PREFIXES(PrerenderTrackerTest, PrerenderTrackerMultiple);

  
  typedef std::map<ChildRouteIdPair, RenderViewInfo> FinalStatusMap;
  
  typedef std::vector<base::WeakPtr<PrerenderResourceThrottle> >
      ResourceThrottleList;
  
  typedef std::map<ChildRouteIdPair, ResourceThrottleList> ResourceThrottleMap;
  struct PendingSwapThrottleData {
    explicit PendingSwapThrottleData(const GURL& swap_url);
    ~PendingSwapThrottleData();
    GURL url;
    std::vector<base::WeakPtr<PrerenderPendingSwapThrottle> > throttles;
  };
  
  typedef std::map<ChildRouteIdPair, PendingSwapThrottleData>
      PendingSwapThrottleMap;

  
  virtual void OnPrerenderStart(PrerenderContents* prerender_contents) OVERRIDE;
  virtual void OnPrerenderStop(PrerenderContents* prerender_contents) OVERRIDE;

  
  
  
  
  
  
  
  
  
  
  
  
  bool SetFinalStatus(int child_id, int route_id,
                      FinalStatus desired_final_status,
                      FinalStatus* actual_final_status);

  
  
  void AddPrerenderOnIOThread(const ChildRouteIdPair& child_route_id_pair);
  void RemovePrerenderOnIOThread(const ChildRouteIdPair& child_route_id_pair,
                                 FinalStatus final_status);

  
  void AddPrerenderPendingSwapOnIOThread(
      const ChildRouteIdPair& child_route_id_pair, const GURL& url);
  void RemovePrerenderPendingSwapOnIOThread(
      const ChildRouteIdPair& child_route_id_pair,
      bool swap_successful);

  
  static void AddPrerenderOnIOThreadTask(
      const ChildRouteIdPair& child_route_id_pair);
  static void RemovePrerenderOnIOThreadTask(
      const ChildRouteIdPair& child_route_id_pair,
      FinalStatus final_status);
  static void AddPrerenderPendingSwapOnIOThreadTask(
      const ChildRouteIdPair& child_route_id_pair, const GURL& url);
  static void RemovePrerenderPendingSwapOnIOThreadTask(
      const ChildRouteIdPair& child_route_id_pair,
      bool swap_successful);

  static PrerenderTracker* GetDefault();

  
  mutable base::Lock final_status_map_lock_;
  
  
  
  FinalStatusMap final_status_map_;

  
  
  
  
  ResourceThrottleMap resource_throttle_io_thread_map_;

  
  
  PendingSwapThrottleMap pending_swap_throttle_map_;

  DISALLOW_COPY_AND_ASSIGN(PrerenderTracker);
};

}  

#endif  

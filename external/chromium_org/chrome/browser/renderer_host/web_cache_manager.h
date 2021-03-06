// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_RENDERER_HOST_WEB_CACHE_MANAGER_H_
#define CHROME_BROWSER_RENDERER_HOST_WEB_CACHE_MANAGER_H_

#include <list>
#include <map>
#include <set>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "third_party/WebKit/public/web/WebCache.h"

template<typename Type>
struct DefaultSingletonTraits;
class PrefRegistrySimple;

class WebCacheManager : public content::NotificationObserver {
  friend class WebCacheManagerTest;
  FRIEND_TEST_ALL_PREFIXES(WebCacheManagerBrowserTest, CrashOnceOnly);

 public:
  static void RegisterPrefs(PrefRegistrySimple* registry);

  
  
  
  static WebCacheManager* GetInstance();

  
  
  void Add(int renderer_id);

  
  
  void Remove(int renderer_id);

  
  
  
  
  
  
  
  void ObserveActivity(int renderer_id);

  
  
  
  void ObserveStats(
      int renderer_id, const blink::WebCache::UsageStats& stats);

  
  size_t global_size_limit() const { return global_size_limit_; }

  
  void SetGlobalSizeLimit(size_t bytes);

  
  void ClearCache();

  
  
  void ClearCacheOnNavigation();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  static size_t GetDefaultGlobalSizeLimit();

 protected:
  
  static const int kRendererInactiveThresholdMinutes = 5;

  
  struct RendererInfo : blink::WebCache::UsageStats {
    
    base::Time access;
  };

  typedef std::map<int, RendererInfo> StatsMap;

  
  
  typedef std::pair<int,size_t> Allocation;

  
  
  typedef std::list<Allocation> AllocationStrategy;

  
  WebCacheManager();
  friend struct DefaultSingletonTraits<WebCacheManager>;

  virtual ~WebCacheManager();

  
  
  void ReviseAllocationStrategy();

  
  void ReviseAllocationStrategyLater();

  
  
  
  
  
  
  
  
  enum AllocationTactic {
    
    
    DIVIDE_EVENLY,

    
    
    KEEP_CURRENT_WITH_HEADROOM,

    
    KEEP_CURRENT,

    
    
    KEEP_LIVE_WITH_HEADROOM,

    
    
    KEEP_LIVE,
  };

  

  
  
  void GatherStats(const std::set<int>& renderers,
                   blink::WebCache::UsageStats* stats);

  
  
  
  static size_t GetSize(AllocationTactic tactic,
                        const blink::WebCache::UsageStats& stats);

  
  
  
  
  
  
  
  bool AttemptTactic(AllocationTactic active_tactic,
                     const blink::WebCache::UsageStats& active_stats,
                     AllocationTactic inactive_tactic,
                     const blink::WebCache::UsageStats& inactive_stats,
                     AllocationStrategy* strategy);

  
  
  
  void AddToStrategy(const std::set<int>& renderers,
                     AllocationTactic tactic,
                     size_t extra_bytes_to_allocate,
                     AllocationStrategy* strategy);

  
  
  void EnactStrategy(const AllocationStrategy& strategy);

  enum ClearCacheOccasion {
    
    INSTANTLY,
    
    
    ON_NAVIGATION
  };

  
  void ClearRendererCache(const std::set<int>& renderers,
                          ClearCacheOccasion occation);

  
  void FindInactiveRenderers();

  
  size_t global_size_limit_;

  
  StatsMap stats_;

  
  
  
  
  std::set<int> active_renderers_;
  
  
  std::set<int> inactive_renderers_;

  base::WeakPtrFactory<WebCacheManager> weak_factory_;

  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(WebCacheManager);
};

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_TIMED_CACHE_H_
#define CHROME_BROWSER_NET_TIMED_CACHE_H_

#include "base/basictypes.h"
#include "base/containers/mru_cache.h"
#include "base/time/time.h"

class GURL;

namespace chrome_browser_net {

class TimedCache {
 public:
  explicit TimedCache(const base::TimeDelta& max_duration);
  ~TimedCache();

  
  
  bool WasRecentlySeen(const GURL& url);

  
  void SetRecentlySeen(const GURL& url);

 private:
  
  
  
  typedef base::MRUCache<GURL, base::TimeTicks> UrlMruTimedCache;
  UrlMruTimedCache mru_cache_;

  
  const base::TimeDelta max_duration_;

  DISALLOW_COPY_AND_ASSIGN(TimedCache);
};

}  

#endif  

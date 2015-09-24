// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREDICTORS_RESOURCE_PREFETCH_COMMON_H_
#define CHROME_BROWSER_PREDICTORS_RESOURCE_PREFETCH_COMMON_H_

#include "base/time/time.h"
#include "url/gurl.h"

class Profile;

namespace content {
class WebContents;
}

namespace predictors {

struct ResourcePrefetchPredictorConfig;

bool IsSpeculativeResourcePrefetchingEnabled(
    Profile* profile,
    ResourcePrefetchPredictorConfig* config);

enum PrefetchKeyType {
  PREFETCH_KEY_TYPE_HOST,
  PREFETCH_KEY_TYPE_URL
};

struct NavigationID {
  
  
  NavigationID();
  NavigationID(const NavigationID& other);
  explicit NavigationID(content::WebContents* web_contents);
  bool operator<(const NavigationID& rhs) const;
  bool operator==(const NavigationID& rhs) const;

  bool IsSameRenderer(const NavigationID& other) const;

  
  
  bool is_valid() const;

  int render_process_id;
  int render_view_id;
  GURL main_frame_url;

  
  
  
  base::TimeTicks creation_time;
};

struct ResourcePrefetchPredictorConfig {
  
  ResourcePrefetchPredictorConfig();
  ~ResourcePrefetchPredictorConfig();

  
  enum Mode {
    URL_LEARNING    = 1 << 0,
    HOST_LEARNING   = 1 << 1,
    URL_PREFETCHING = 1 << 2,  
    HOST_PRFETCHING = 1 << 3   
  };
  int mode;

  
  bool IsLearningEnabled() const;
  bool IsPrefetchingEnabled() const;
  bool IsURLLearningEnabled() const;
  bool IsHostLearningEnabled() const;
  bool IsURLPrefetchingEnabled() const;
  bool IsHostPrefetchingEnabled() const;

  
  
  int max_navigation_lifetime_seconds;

  
  int max_urls_to_track;
  int max_hosts_to_track;

  
  
  
  int min_url_visit_count;

  
  int max_resources_per_entry;
  
  int max_consecutive_misses;

  
  
  float min_resource_confidence_to_trigger_prefetch;
  
  int min_resource_hits_to_trigger_prefetch;

  
  int max_prefetches_inflight_per_navigation;
  
  
  int max_prefetches_inflight_per_host_per_navigation;
};

}  

#endif  

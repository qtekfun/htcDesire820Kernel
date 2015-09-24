// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MANAGED_MODE_MANAGED_MODE_URL_FILTER_H_
#define CHROME_BROWSER_MANAGED_MODE_MANAGED_MODE_URL_FILTER_H_

#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/observer_list.h"
#include "base/threading/non_thread_safe.h"
#include "base/values.h"
#include "chrome/browser/managed_mode/managed_mode_site_list.h"
#include "chrome/browser/managed_mode/managed_users.h"

class GURL;

class ManagedModeURLFilter
    : public base::RefCountedThreadSafe<ManagedModeURLFilter>,
      public base::NonThreadSafe {
 public:
  enum FilteringBehavior {
    ALLOW,
    WARN,
    BLOCK,
    HISTOGRAM_BOUNDING_VALUE
  };

  class Observer {
   public:
    virtual void OnSiteListUpdated() = 0;
  };

  struct Contents;

  ManagedModeURLFilter();

  static FilteringBehavior BehaviorFromInt(int behavior_value);

  
  static GURL Normalize(const GURL& url);

  
  
  
  static bool HasFilteredScheme(const GURL& url);

  
  
  
  
  
  
  
  
  
  
  
  
  static bool HostMatchesPattern(const std::string& host,
                                 const std::string& pattern);

  void GetSites(const GURL& url,
                std::vector<ManagedModeSiteList::Site*>* sites) const;

  
  
  FilteringBehavior GetFilteringBehaviorForURL(const GURL& url) const;

  
  void SetDefaultFilteringBehavior(FilteringBehavior behavior);

  
  
  
  void LoadWhitelists(ScopedVector<ManagedModeSiteList> site_lists);

  
  
  void SetFromPatterns(const std::vector<std::string>& patterns);

  
  void SetManualHosts(const std::map<std::string, bool>* host_map);

  
  void SetManualURLs(const std::map<GURL, bool>* url_map);

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

 private:
  friend class base::RefCountedThreadSafe<ManagedModeURLFilter>;
  ~ManagedModeURLFilter();

  void SetContents(scoped_ptr<Contents> url_matcher);

  ObserverList<Observer> observers_;

  FilteringBehavior default_behavior_;
  scoped_ptr<Contents> contents_;

  
  
  std::map<GURL, bool> url_map_;

  
  
  std::map<std::string, bool> host_map_;

  DISALLOW_COPY_AND_ASSIGN(ManagedModeURLFilter);
};

#endif  

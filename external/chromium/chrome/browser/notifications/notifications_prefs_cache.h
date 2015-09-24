// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_NOTIFICATIONS_PREFS_CACHE_H_
#define CHROME_BROWSER_NOTIFICATIONS_NOTIFICATIONS_PREFS_CACHE_H_
#pragma once

#include <set>
#include <vector>

#include "base/memory/ref_counted.h"
#include "chrome/common/content_settings.h"
#include "googleurl/src/gurl.h"

class ListValue;

class NotificationsPrefsCache
    : public base::RefCountedThreadSafe<NotificationsPrefsCache> {
 public:
  NotificationsPrefsCache();

  
  
  void set_is_initialized(bool val) { is_initialized_ = val; }
  bool is_initialized() { return is_initialized_; }

  
  
  
  int HasPermission(const GURL& origin);

  
  void CacheAllowedOrigin(const GURL& origin);
  void CacheDeniedOrigin(const GURL& origin);

  
  
  void SetCacheAllowedOrigins(const std::vector<GURL>& allowed);
  void SetCacheDeniedOrigins(const std::vector<GURL>& denied);
  void SetCacheDefaultContentSetting(ContentSetting setting);

  static void ListValueToGurlVector(const ListValue& origin_list,
                                    std::vector<GURL>* origin_vector);

  
  ContentSetting CachedDefaultContentSetting() {
    return default_content_setting_;
  }

 private:
  friend class base::RefCountedThreadSafe<NotificationsPrefsCache>;

  virtual ~NotificationsPrefsCache();

  
  bool IsOriginAllowed(const GURL& origin);
  bool IsOriginDenied(const GURL& origin);

  
  void CheckThreadAccess();

  
  std::set<GURL> allowed_origins_;
  std::set<GURL> denied_origins_;

  
  
  ContentSetting default_content_setting_;

  
  
  bool is_initialized_;

  DISALLOW_COPY_AND_ASSIGN(NotificationsPrefsCache);
};

#endif  

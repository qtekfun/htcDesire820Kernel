// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_THROTTLER_MANAGER_H_
#define NET_URL_REQUEST_URL_REQUEST_THROTTLER_MANAGER_H_
#pragma once

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "base/threading/non_thread_safe.h"
#include "googleurl/src/gurl.h"
#include "net/url_request/url_request_throttler_entry.h"

namespace net {

class URLRequestThrottlerManager : public base::NonThreadSafe {
 public:
  static URLRequestThrottlerManager* GetInstance();

  
  
  
  
  scoped_refptr<URLRequestThrottlerEntryInterface> RegisterRequestUrl(
      const GURL& url);

  
  
  
  void AddToOptOutList(const std::string& host);

  
  
  
  void OverrideEntryForTests(const GURL& url, URLRequestThrottlerEntry* entry);

  
  
  
  
  void EraseEntryForTests(const GURL& url);

  
  
  
  void set_enable_thread_checks(bool enable);
  bool enable_thread_checks() const;

  
  void set_enforce_throttling(bool enforce);
  bool enforce_throttling();

 protected:
  URLRequestThrottlerManager();
  ~URLRequestThrottlerManager();

  
  
  
  
  
  std::string GetIdFromUrl(const GURL& url) const;

  
  
  
  void GarbageCollectEntriesIfNecessary();

  
  void GarbageCollectEntries();

  
  int GetNumberOfEntriesForTests() const { return url_entries_.size(); }

 private:
  friend struct DefaultSingletonTraits<URLRequestThrottlerManager>;

  
  
  typedef std::map<std::string, scoped_refptr<URLRequestThrottlerEntry> >
      UrlEntryMap;

  
  
  typedef std::set<std::string> OptOutHosts;

  
  static const unsigned int kMaximumNumberOfEntries;
  
  static const unsigned int kRequestsBetweenCollecting;

  
  
  UrlEntryMap url_entries_;

  
  OptOutHosts opt_out_hosts_;

  
  
  unsigned int requests_since_last_gc_;

  
  GURL::Replacements url_id_replacements_;

  
  
  bool enforce_throttling_;

  
  
  
  
  
  
  bool enable_thread_checks_;

  DISALLOW_COPY_AND_ASSIGN(URLRequestThrottlerManager);
};

}  

#endif  

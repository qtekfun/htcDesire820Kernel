// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_THROTTLER_MANAGER_H_
#define NET_URL_REQUEST_URL_REQUEST_THROTTLER_MANAGER_H_

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/threading/non_thread_safe.h"
#include "base/threading/platform_thread.h"
#include "net/base/net_export.h"
#include "net/base/network_change_notifier.h"
#include "net/url_request/url_request_throttler_entry.h"
#include "url/gurl.h"

namespace net {

class BoundNetLog;
class NetLog;

class NET_EXPORT URLRequestThrottlerManager
    : NON_EXPORTED_BASE(public base::NonThreadSafe),
      public NetworkChangeNotifier::IPAddressObserver,
      public NetworkChangeNotifier::ConnectionTypeObserver {
 public:
  URLRequestThrottlerManager();
  virtual ~URLRequestThrottlerManager();

  
  
  
  
  scoped_refptr<URLRequestThrottlerEntryInterface> RegisterRequestUrl(
      const GURL& url);

  
  
  
  void AddToOptOutList(const std::string& host);

  
  
  
  void OverrideEntryForTests(const GURL& url, URLRequestThrottlerEntry* entry);

  
  
  
  
  void EraseEntryForTests(const GURL& url);

  
  
  
  void set_enable_thread_checks(bool enable);
  bool enable_thread_checks() const;

  
  void set_enforce_throttling(bool enforce);
  bool enforce_throttling();

  
  void set_net_log(NetLog* net_log);
  NetLog* net_log() const;

  
  virtual void OnIPAddressChanged() OVERRIDE;

  
  virtual void OnConnectionTypeChanged(
      NetworkChangeNotifier::ConnectionType type) OVERRIDE;

  
  
  
  
  
  std::string GetIdFromUrl(const GURL& url) const;

  
  
  
  void GarbageCollectEntriesIfNecessary();

  
  void GarbageCollectEntries();

  
  
  
  
  // responses are from a badly-written proxy that should have returned a
  
  
  void OnNetworkChange();

  
  int GetNumberOfEntriesForTests() const { return url_entries_.size(); }

 private:
  
  
  typedef std::map<std::string, scoped_refptr<URLRequestThrottlerEntry> >
      UrlEntryMap;

  
  
  typedef std::set<std::string> OptOutHosts;

  
  static const unsigned int kMaximumNumberOfEntries;
  
  static const unsigned int kRequestsBetweenCollecting;

  
  
  UrlEntryMap url_entries_;

  
  OptOutHosts opt_out_hosts_;

  
  
  unsigned int requests_since_last_gc_;

  
  GURL::Replacements url_id_replacements_;

  
  
  
  
  
  
  bool enable_thread_checks_;

  
  
  bool logged_for_localhost_disabled_;

  
  BoundNetLog net_log_;

  
  base::PlatformThreadId registered_from_thread_;

  DISALLOW_COPY_AND_ASSIGN(URLRequestThrottlerManager);
};

}  

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INTRANET_REDIRECT_DETECTOR_H_
#define CHROME_BROWSER_INTRANET_REDIRECT_DETECTOR_H_
#pragma once

#include <set>
#include <string>
#include <vector>

#include "chrome/common/net/url_fetcher.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "googleurl/src/gurl.h"
#include "net/base/host_resolver_proc.h"
#include "net/base/network_change_notifier.h"

class PrefService;

class IntranetRedirectDetector
    : public URLFetcher::Delegate,
      public net::NetworkChangeNotifier::IPAddressObserver {
 public:
  
  
  
  
  
  
  IntranetRedirectDetector();
  ~IntranetRedirectDetector();

  
  
  static GURL RedirectOrigin();

  static void RegisterPrefs(PrefService* prefs);

  
  
  static const size_t kNumCharsInHostnames;

 private:
  typedef std::set<URLFetcher*> Fetchers;

  
  
  void FinishSleep();

  
  virtual void OnURLFetchComplete(const URLFetcher* source,
                                  const GURL& url,
                                  const net::URLRequestStatus& status,
                                  int response_code,
                                  const ResponseCookies& cookies,
                                  const std::string& data);

  
  virtual void OnIPAddressChanged();

  GURL redirect_origin_;
  ScopedRunnableMethodFactory<IntranetRedirectDetector> fetcher_factory_;
  Fetchers fetchers_;
  std::vector<GURL> resulting_origins_;
  bool in_sleep_;  
                   
                   

  DISALLOW_COPY_AND_ASSIGN(IntranetRedirectDetector);
};

class IntranetRedirectHostResolverProc : public net::HostResolverProc {
 public:
  explicit IntranetRedirectHostResolverProc(net::HostResolverProc* previous);

  virtual int Resolve(const std::string& host,
                      net::AddressFamily address_family,
                      net::HostResolverFlags host_resolver_flags,
                      net::AddressList* addrlist,
                      int* os_error);
};

#endif  

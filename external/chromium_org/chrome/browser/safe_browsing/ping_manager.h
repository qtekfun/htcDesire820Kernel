// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_PING_MANAGER_H_
#define CHROME_BROWSER_SAFE_BROWSING_PING_MANAGER_H_

#include <set>
#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/safe_browsing/protocol_manager_helper.h"
#include "chrome/browser/safe_browsing/safe_browsing_util.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "url/gurl.h"

namespace net {
class URLRequestContextGetter;
}  


class SafeBrowsingPingManager : public net::URLFetcherDelegate {
 public:
  virtual ~SafeBrowsingPingManager();

  
  static SafeBrowsingPingManager* Create(
      net::URLRequestContextGetter* request_context_getter,
      const SafeBrowsingProtocolConfig& config);

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  
  
  void ReportSafeBrowsingHit(const GURL& malicious_url,
                             const GURL& page_url,
                             const GURL& referrer_url,
                             bool is_subresource,
                             SBThreatType threat_type,
                             const std::string& post_data);

  
  
  void ReportMalwareDetails(const std::string& report);

 private:
  FRIEND_TEST_ALL_PREFIXES(SafeBrowsingPingManagerTest,
                           TestSafeBrowsingHitUrl);
  FRIEND_TEST_ALL_PREFIXES(SafeBrowsingPingManagerTest,
                           TestMalwareDetailsUrl);

  typedef std::set<const net::URLFetcher*> Reports;

  
  
  SafeBrowsingPingManager(
      net::URLRequestContextGetter* request_context_getter,
      const SafeBrowsingProtocolConfig& config);

  
  GURL SafeBrowsingHitUrl(
      const GURL& malicious_url, const GURL& page_url, const GURL& referrer_url,
      bool is_subresource,
      SBThreatType threat_type) const;
  
  GURL MalwareDetailsUrl() const;

  
  std::string version_;

  
  std::string client_name_;

  
  scoped_refptr<net::URLRequestContextGetter> request_context_getter_;

  
  
  std::string url_prefix_;

  
  
  Reports safebrowsing_reports_;

  DISALLOW_COPY_AND_ASSIGN(SafeBrowsingPingManager);
};

#endif  

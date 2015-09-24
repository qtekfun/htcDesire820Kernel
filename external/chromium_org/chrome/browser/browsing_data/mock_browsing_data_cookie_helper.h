// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSING_DATA_MOCK_BROWSING_DATA_COOKIE_HELPER_H_
#define CHROME_BROWSER_BROWSING_DATA_MOCK_BROWSING_DATA_COOKIE_HELPER_H_

#include <map>
#include <string>

#include "chrome/browser/browsing_data/browsing_data_cookie_helper.h"
#include "net/cookies/canonical_cookie.h"

class MockBrowsingDataCookieHelper : public BrowsingDataCookieHelper {
 public:
  explicit MockBrowsingDataCookieHelper(
      net::URLRequestContextGetter* request_context_getter);

  
  virtual void StartFetching(
      const net::CookieMonster::GetCookieListCallback &callback) OVERRIDE;
  virtual void DeleteCookie(const net::CanonicalCookie& cookie) OVERRIDE;

  
  void AddCookieSamples(const GURL& url, const std::string& cookie_line);

  
  void Notify();

  
  void Reset();

  
  
  bool AllDeleted();

 private:
  virtual ~MockBrowsingDataCookieHelper();

  net::CookieMonster::GetCookieListCallback callback_;

  net::CookieList cookie_list_;

  
  std::map<const std::string, bool> cookies_;
};

#endif  

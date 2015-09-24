// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILE_RESETTER_BRANDCODE_CONFIG_FETCHER_H_
#define CHROME_BROWSER_PROFILE_RESETTER_BRANDCODE_CONFIG_FETCHER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/timer/timer.h"
#include "net/url_request/url_fetcher_delegate.h"

class BrandcodedDefaultSettings;
class GURL;

class BrandcodeConfigFetcher : public net::URLFetcherDelegate {
 public:
  typedef base::Callback<void ()> FetchCallback;

  BrandcodeConfigFetcher(const FetchCallback& callback,
                         const GURL& url,
                         const std::string& brandcode);
  virtual ~BrandcodeConfigFetcher();

  bool IsActive() const { return config_fetcher_; }

  scoped_ptr<BrandcodedDefaultSettings> GetSettings() {
    return default_settings_.Pass();
  }

  
  void SetCallback(const FetchCallback& callback);

 private:
  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  void OnDownloadTimeout();

  
  
  base::OneShotTimer<BrandcodeConfigFetcher> download_timer_;

  
  FetchCallback fetch_callback_;

  
  scoped_ptr<net::URLFetcher> config_fetcher_;

  
  scoped_ptr<BrandcodedDefaultSettings> default_settings_;

  DISALLOW_COPY_AND_ASSIGN(BrandcodeConfigFetcher);
};

#endif  

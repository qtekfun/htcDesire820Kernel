// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CAPTIVE_PORTAL_CAPTIVE_PORTAL_DETECTOR_H_
#define CHROME_BROWSER_CAPTIVE_PORTAL_CAPTIVE_PORTAL_DETECTOR_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "net/url_request/url_fetcher.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "net/url_request/url_request_context_getter.h"

class GURL;

namespace captive_portal {

enum Result {
  
  RESULT_INTERNET_CONNECTED,
  
  RESULT_NO_RESPONSE,
  
  
  RESULT_BEHIND_CAPTIVE_PORTAL,
  RESULT_COUNT
};

class CaptivePortalDetector : public net::URLFetcherDelegate,
                              public base::NonThreadSafe {
 public:
  struct Results {
    Results()
        : result(RESULT_NO_RESPONSE),
          response_code(net::URLFetcher::RESPONSE_CODE_INVALID) {
    }

    Result result;
    int response_code;
    base::TimeDelta retry_after_delta;
  };

  typedef base::Callback<void(const Results& results)> DetectionCallback;

  
  
  
  
  
  static const char kDefaultURL[];

  explicit CaptivePortalDetector(
      const scoped_refptr<net::URLRequestContextGetter>& request_context);
  virtual ~CaptivePortalDetector();

  static std::string CaptivePortalResultToString(Result result);

  
  
  void DetectCaptivePortal(const GURL& url, const DetectionCallback& callback);

  
  void Cancel();

 private:
  friend class CaptivePortalDetectorTestBase;

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  
  
  
  
  void GetCaptivePortalResultFromResponse(const net::URLFetcher* url_fetcher,
                                          Results* results) const;

  
  
  base::Time GetCurrentTime() const;

  
  bool FetchingURL() const;

  
  void set_time_for_testing(const base::Time& time) {
    time_for_testing_ = time;
  }

  
  void advance_time_for_testing(const base::TimeDelta& delta) {
    time_for_testing_ += delta;
  }

  
  scoped_refptr<net::URLRequestContextGetter> request_context_;

  DetectionCallback detection_callback_;

  scoped_ptr<net::URLFetcher> url_fetcher_;

  
  base::Time time_for_testing_;

  DISALLOW_COPY_AND_ASSIGN(CaptivePortalDetector);
};

}  

#endif  

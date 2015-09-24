// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_THROTTLER_TEST_SUPPORT_H_
#define NET_URL_REQUEST_URL_REQUEST_THROTTLER_TEST_SUPPORT_H_

#include <string>

#include "base/time/time.h"
#include "net/base/backoff_entry.h"
#include "net/url_request/url_request_throttler_header_interface.h"

namespace net {

class MockBackoffEntry : public BackoffEntry {
 public:
  explicit MockBackoffEntry(const BackoffEntry::Policy* const policy);
  virtual ~MockBackoffEntry();

  
  virtual base::TimeTicks ImplGetTimeNow() const OVERRIDE;

  void set_fake_now(const base::TimeTicks& now);

 private:
  base::TimeTicks fake_now_;
};

class MockURLRequestThrottlerHeaderAdapter
    : public URLRequestThrottlerHeaderInterface {
 public:
  
  
  explicit MockURLRequestThrottlerHeaderAdapter(int response_code);

  
  
  
  MockURLRequestThrottlerHeaderAdapter(const std::string& retry_value,
                                       const std::string& opt_out_value,
                                       int response_code);
  virtual ~MockURLRequestThrottlerHeaderAdapter();

  
  virtual std::string GetNormalizedValue(const std::string& key) const OVERRIDE;
  virtual int GetResponseCode() const OVERRIDE;

 private:
  std::string fake_retry_value_;
  std::string fake_opt_out_value_;
  int fake_response_code_;

  DISALLOW_COPY_AND_ASSIGN(MockURLRequestThrottlerHeaderAdapter);
};

}  

#endif  

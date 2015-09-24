// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_CONNECT_INTERCEPTOR_H_
#define CHROME_BROWSER_NET_CONNECT_INTERCEPTOR_H_

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/net/timed_cache.h"

class GURL;

namespace net {
class URLRequest;
}

namespace chrome_browser_net {

class Predictor;

class ConnectInterceptor {
 public:
  
  explicit ConnectInterceptor(Predictor* predictor);
  
  virtual ~ConnectInterceptor();

  
  void WitnessURLRequest(net::URLRequest* request);

 private:
  
  FRIEND_TEST_ALL_PREFIXES(ConnectInterceptorTest, TimedCacheRecall);
  FRIEND_TEST_ALL_PREFIXES(ConnectInterceptorTest, TimedCacheEviction);

  TimedCache timed_cache_;
  Predictor* const predictor_;

  DISALLOW_COPY_AND_ASSIGN(ConnectInterceptor);
};

}  

#endif  

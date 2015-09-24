// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_COMPONENT_UPDATER_TEST_URL_REQUEST_POST_INTERCEPTOR_H_
#define CHROME_BROWSER_COMPONENT_UPDATER_TEST_URL_REQUEST_POST_INTERCEPTOR_H_

#include <map>
#include <queue>
#include <string>
#include <utility>
#include <vector>
#include "base/basictypes.h"
#include "base/synchronization/lock.h"
#include "url/gurl.h"

namespace base {
class FilePath;
}

namespace net {
class URLRequest;
}

namespace component_updater {

class URLRequestPostInterceptor {
 public:
  
  class RequestMatcher {
   public:
    virtual bool Match(const std::string& actual) const = 0;
    virtual ~RequestMatcher() {}
  };

  
  GURL GetUrl() const;

  
  
  
  
  
  bool ExpectRequest(class RequestMatcher* request_matcher);
  bool ExpectRequest(class RequestMatcher* request_matcher,
                     const base::FilePath& filepath);

  
  
  int GetHitCount() const;

  
  int GetCount() const;

  
  std::vector<std::string> GetRequests() const;

  
  std::string GetRequestsAsString() const;

  
  void Reset();

  class Delegate;

 private:
  friend class URLRequestPostInterceptorFactory;
  typedef std::pair<const RequestMatcher*, std::string> Expectation;

  explicit URLRequestPostInterceptor(const GURL& url);
  ~URLRequestPostInterceptor();

  void ClearExpectations();
  const GURL url_;

  mutable base::Lock interceptor_lock_;
  mutable int hit_count_;
  mutable std::vector<std::string> requests_;
  mutable std::queue<Expectation> expectations_;

  DISALLOW_COPY_AND_ASSIGN(URLRequestPostInterceptor);
};

class URLRequestPostInterceptorFactory {
 public:
  URLRequestPostInterceptorFactory(const std::string& scheme,
                                   const std::string& hostname);
  ~URLRequestPostInterceptorFactory();

  
  
  
  URLRequestPostInterceptor* CreateInterceptor(const base::FilePath& filepath);

 private:
  const std::string scheme_;
  const std::string hostname_;

  
  
  
  URLRequestPostInterceptor::Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(URLRequestPostInterceptorFactory);
};

}  

#endif  

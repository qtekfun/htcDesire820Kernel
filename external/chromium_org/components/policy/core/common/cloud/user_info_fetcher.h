// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CLOUD_USER_INFO_FETCHER_H_
#define COMPONENTS_POLICY_CORE_COMMON_CLOUD_USER_INFO_FETCHER_H_

#include <string>
#include "base/memory/scoped_ptr.h"
#include "components/policy/policy_export.h"
#include "net/url_request/url_fetcher_delegate.h"

class GoogleServiceAuthError;

namespace base {
class DictionaryValue;
}

namespace net {
class URLFetcher;
class URLRequestContextGetter;
}

namespace policy {

class POLICY_EXPORT UserInfoFetcher : public net::URLFetcherDelegate {
 public:
  class POLICY_EXPORT Delegate {
   public:
    
    
    
    virtual void OnGetUserInfoSuccess(
        const base::DictionaryValue* response) = 0;

    
    
    
    virtual void OnGetUserInfoFailure(const GoogleServiceAuthError& error) = 0;
  };

  
  UserInfoFetcher(Delegate* delegate, net::URLRequestContextGetter* context);
  virtual ~UserInfoFetcher();

  
  void Start(const std::string& access_token);

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

 private:
  Delegate* delegate_;
  net::URLRequestContextGetter* context_;
  scoped_ptr<net::URLFetcher> url_fetcher_;

  DISALLOW_COPY_AND_ASSIGN(UserInfoFetcher);
};

}  

#endif  

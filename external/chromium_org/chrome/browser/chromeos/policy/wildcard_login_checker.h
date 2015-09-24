// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_WILDCARD_LOGIN_CHECKER_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_WILDCARD_LOGIN_CHECKER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "components/policy/core/common/cloud/user_info_fetcher.h"
#include "google_apis/gaia/google_service_auth_error.h"

namespace net {
class URLRequestContextGetter;
}

namespace policy {

class PolicyOAuth2TokenFetcher;

class WildcardLoginChecker : public UserInfoFetcher::Delegate {
 public:
  typedef base::Callback<void(bool)> StatusCallback;

  WildcardLoginChecker();
  virtual ~WildcardLoginChecker();

  
  void Start(scoped_refptr<net::URLRequestContextGetter> signin_context,
             const StatusCallback& callback);

  
  void StartWithAccessToken(const std::string& access_token,
                            const StatusCallback& callback);

  
  virtual void OnGetUserInfoSuccess(const base::DictionaryValue* response)
      OVERRIDE;
  virtual void OnGetUserInfoFailure(const GoogleServiceAuthError& error)
      OVERRIDE;

 private:
  
  void OnPolicyTokenFetched(const std::string& access_token,
                            const GoogleServiceAuthError& error);

  
  void StartUserInfoFetcher(const std::string& access_token);

  
  void OnCheckCompleted(bool result);

  StatusCallback callback_;

  scoped_ptr<PolicyOAuth2TokenFetcher> token_fetcher_;
  scoped_ptr<UserInfoFetcher> user_info_fetcher_;

  DISALLOW_COPY_AND_ASSIGN(WildcardLoginChecker);
};

}  

#endif  

// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_OAUTH2_LOGIN_VERIFIER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_OAUTH2_LOGIN_VERIFIER_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/profiles/profile.h"
#include "google_apis/gaia/gaia_auth_consumer.h"
#include "google_apis/gaia/gaia_auth_fetcher.h"
#include "google_apis/gaia/oauth2_access_token_fetcher.h"
#include "google_apis/gaia/oauth2_token_service.h"
#include "net/url_request/url_request_context_getter.h"

namespace chromeos {

class OAuth2LoginVerifier : public base::SupportsWeakPtr<OAuth2LoginVerifier>,
                            public GaiaAuthConsumer,
                            public OAuth2TokenService::Consumer {
 public:
  typedef base::Callback<void(bool connection_error)> ErrorHandler;

  class Delegate {
   public:
    virtual ~Delegate() {}
    
    virtual void OnSessionMergeSuccess() = 0;

    
    virtual void OnSessionMergeFailure(bool connection_error) = 0;

    
    
    virtual void OnListAccountsSuccess(const std::string& data) = 0;

    
    virtual void OnListAccountsFailure(bool connection_error) = 0;
  };

  OAuth2LoginVerifier(OAuth2LoginVerifier::Delegate* delegate,
                      net::URLRequestContextGetter* system_request_context,
                      net::URLRequestContextGetter* user_request_context);
  virtual ~OAuth2LoginVerifier();

  
  
  void VerifyProfileTokens(Profile* profile);

 private:
  enum SessionRestoreType {
    RESTORE_UNDEFINED = 0,
    RESTORE_FROM_GAIA_TOKEN = 1,
    RESTORE_FROM_OAUTH2_REFRESH_TOKEN = 2,
  };
  
  virtual void OnUberAuthTokenSuccess(const std::string& token) OVERRIDE;
  virtual void OnUberAuthTokenFailure(
      const GoogleServiceAuthError& error) OVERRIDE;
  virtual void OnMergeSessionSuccess(const std::string& data) OVERRIDE;
  virtual void OnMergeSessionFailure(
      const GoogleServiceAuthError& error) OVERRIDE;
  virtual void OnListAccountsSuccess(const std::string& data) OVERRIDE;
  virtual void OnListAccountsFailure(
      const GoogleServiceAuthError& error) OVERRIDE;

  
  virtual void OnGetTokenSuccess(const OAuth2TokenService::Request* request,
                                 const std::string& access_token,
                                 const base::Time& expiration_time) OVERRIDE;
  virtual void OnGetTokenFailure(const OAuth2TokenService::Request* request,
                                 const GoogleServiceAuthError& error) OVERRIDE;

  
  void StartFetchingOAuthLoginAccessToken(Profile* profile);

  
  void StartOAuthLoginForUberToken();

  
  void StartMergeSession();

  
  
  void SchedulePostMergeVerification();

  
  void StartPostRestoreVerification();

  
  
  void RetryOnError(const char* operation_id,
                    const GoogleServiceAuthError& error,
                    const base::Closure& task_to_retry,
                    const ErrorHandler& error_handler);

  OAuth2LoginVerifier::Delegate* delegate_;
  scoped_refptr<net::URLRequestContextGetter> system_request_context_;
  scoped_refptr<net::URLRequestContextGetter> user_request_context_;
  scoped_ptr<GaiaAuthFetcher> gaia_fetcher_;
  std::string access_token_;
  std::string gaia_token_;
  scoped_ptr<OAuth2TokenService::Request> login_token_request_;
  
  int retry_count_;

  DISALLOW_COPY_AND_ASSIGN(OAuth2LoginVerifier);
};

}  

#endif  

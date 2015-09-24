// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SIGNIN_FAKE_PROFILE_OAUTH2_TOKEN_SERVICE_H_
#define CHROME_BROWSER_SIGNIN_FAKE_PROFILE_OAUTH2_TOKEN_SERVICE_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"

#if defined(OS_ANDROID)
#include "chrome/browser/signin/android_profile_oauth2_token_service.h"
#else
#include "chrome/browser/signin/profile_oauth2_token_service.h"
#endif

namespace content {
class BrowserContext;
}

class FakeProfileOAuth2TokenService
#if defined(OS_ANDROID)
  : public AndroidProfileOAuth2TokenService {
#else
  : public ProfileOAuth2TokenService {
#endif
 public:
  struct PendingRequest {
    PendingRequest();
    ~PendingRequest();

    std::string account_id;
    std::string client_id;
    std::string client_secret;
    ScopeSet scopes;
    base::WeakPtr<RequestImpl> request;
  };

  FakeProfileOAuth2TokenService();
  virtual ~FakeProfileOAuth2TokenService();

  
  virtual bool RefreshTokenIsAvailable(
      const std::string& account_id) OVERRIDE;

  
  
  virtual void UpdateCredentials(const std::string& account_id,
                                 const std::string& refresh_token) OVERRIDE;

  
  
  
  void IssueRefreshToken(const std::string& token);

  
  
  void IssueRefreshTokenForUser(const std::string& account_id,
                                const std::string& token);

  
  
  std::vector<PendingRequest> GetPendingRequests();

  
  
  void IssueTokenForScope(const ScopeSet& scopes,
                          const std::string& access_token,
                          const base::Time& expiration);

  void IssueErrorForScope(const ScopeSet& scopes,
                          const GoogleServiceAuthError& error);

  void IssueTokenForAllPendingRequests(const std::string& access_token,
                                       const base::Time& expiration);

  void IssueErrorForAllPendingRequests(const GoogleServiceAuthError& error);

  
  
  static BrowserContextKeyedService* Build(content::BrowserContext* profile);

 protected:
  
  virtual void FetchOAuth2Token(RequestImpl* request,
                                const std::string& account_id,
                                net::URLRequestContextGetter* getter,
                                const std::string& client_id,
                                const std::string& client_secret,
                                const ScopeSet& scopes) OVERRIDE;

  virtual void InvalidateOAuth2Token(const std::string& account_id,
                                     const std::string& client_id,
                                     const ScopeSet& scopes,
                                     const std::string& access_token) OVERRIDE;

  virtual std::string GetRefreshToken(const std::string& account_id) OVERRIDE;

  virtual net::URLRequestContextGetter* GetRequestContext() OVERRIDE;

  virtual void RevokeCredentialsOnServer(
      const std::string& refresh_token) OVERRIDE;

 private:
  
  
  
  void CompleteRequests(bool all_scopes,
                        const ScopeSet& scopes,
                        const GoogleServiceAuthError& error,
                        const std::string& access_token,
                        const base::Time& expiration);

  std::vector<PendingRequest> pending_requests_;
  std::string refresh_token_;

  DISALLOW_COPY_AND_ASSIGN(FakeProfileOAuth2TokenService);
};

#endif  

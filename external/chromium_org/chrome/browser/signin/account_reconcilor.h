// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef CHROME_BROWSER_SIGNIN_ACCOUNT_RECONCILOR_H_
#define CHROME_BROWSER_SIGNIN_ACCOUNT_RECONCILOR_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "google_apis/gaia/gaia_auth_consumer.h"
#include "google_apis/gaia/oauth2_token_service.h"

class GaiaAuthFetcher;
class Profile;
struct ChromeCookieDetails;

class AccountReconcilor : public BrowserContextKeyedService,
                                 content::NotificationObserver,
                                 GaiaAuthConsumer,
                                 OAuth2TokenService::Consumer,
                                 OAuth2TokenService::Observer {
 public:
  explicit AccountReconcilor(Profile* profile);
  virtual ~AccountReconcilor();

  
  virtual void Shutdown() OVERRIDE;

  Profile* profile() { return profile_; }

  bool IsPeriodicReconciliationRunning() {
    return reconciliation_timer_.IsRunning();
  }

 private:
  class AccountReconcilorTest;

  
  void RegisterWithCookieMonster();
  void UnregisterWithCookieMonster();
  void RegisterWithSigninManager();
  void UnregisterWithSigninManager();
  void RegisterWithTokenService();
  void UnregisterWithTokenService();

  bool IsProfileConnected();

  void DeleteAccessTokenRequests();

  
  void StartPeriodicReconciliation();
  void StopPeriodicReconciliation();
  void PeriodicReconciliation();

  void PerformMergeAction(const std::string& account_id);
  void PerformRemoveAction(const std::string& account_id);
  void StartReconcileAction();
  void FinishReconcileAction();

  void GetAccountsFromCookie();
  void ValidateAccountsFromTokenService();

  void OnCookieChanged(ChromeCookieDetails* details);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void OnGetTokenSuccess(const OAuth2TokenService::Request* request,
                                 const std::string& access_token,
                                 const base::Time& expiration_time) OVERRIDE;
  virtual void OnGetTokenFailure(const OAuth2TokenService::Request* request,
                                 const GoogleServiceAuthError& error) OVERRIDE;

  
  virtual void OnRefreshTokenAvailable(const std::string& account_id) OVERRIDE;
  virtual void OnRefreshTokenRevoked(const std::string& account_id) OVERRIDE;
  virtual void OnRefreshTokensLoaded() OVERRIDE;

  
  virtual void OnListAccountsSuccess(const std::string& data) OVERRIDE;
  virtual void OnListAccountsFailure(
      const GoogleServiceAuthError& error) OVERRIDE;

  
  Profile* profile_;
  content::NotificationRegistrar registrar_;
  base::RepeatingTimer<AccountReconcilor> reconciliation_timer_;

  
  
  scoped_ptr<GaiaAuthFetcher> gaia_fetcher_;
  bool are_gaia_accounts_set_;
  std::vector<std::string> gaia_accounts_;

  
  
  std::string primary_account_;
  std::vector<std::string> chrome_accounts_;
  scoped_ptr<OAuth2TokenService::Request>* requests_;
  std::set<std::string> valid_chrome_accounts_;
  std::set<std::string> invalid_chrome_accounts_;

  DISALLOW_COPY_AND_ASSIGN(AccountReconcilor);
};

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_ONLINE_ATTEMPT_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_ONLINE_ATTEMPT_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/login/login_status_consumer.h"
#include "google_apis/gaia/gaia_auth_consumer.h"
#include "google_apis/gaia/google_service_auth_error.h"

class GaiaAuthFetcher;
class Profile;

namespace chromeos {
class AuthAttemptState;
class AuthAttemptStateResolver;

class OnlineAttempt
    : public GaiaAuthConsumer {
 public:
  OnlineAttempt(AuthAttemptState* current_attempt,
                AuthAttemptStateResolver* callback);
  virtual ~OnlineAttempt();

  
  
  
  
  void Initiate(Profile* auth_profile);

  
  virtual void OnClientLoginFailure(
      const GoogleServiceAuthError& error) OVERRIDE;
  virtual void OnClientLoginSuccess(
      const GaiaAuthConsumer::ClientLoginResult& credentials) OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(OnlineAttemptTest, LoginSuccess);
  FRIEND_TEST_ALL_PREFIXES(OnlineAttemptTest, TwoFactorSuccess);

  
  static const int kClientLoginTimeoutMs;

  void TryClientLogin();
  void CancelClientLogin();

  void TriggerResolve(const LoginFailure& outcome);

  bool HasPendingFetch();
  void CancelRequest();

  AuthAttemptState* const attempt_;
  AuthAttemptStateResolver* const resolver_;

  
  scoped_ptr<GaiaAuthFetcher> client_fetcher_;

  
  base::WeakPtrFactory<OnlineAttempt> weak_factory_;

  
  bool try_again_;

  friend class OnlineAttemptTest;
  DISALLOW_COPY_AND_ASSIGN(OnlineAttempt);
};

}  

#endif  

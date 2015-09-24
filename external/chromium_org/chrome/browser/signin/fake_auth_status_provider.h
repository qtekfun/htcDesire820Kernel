// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SIGNIN_FAKE_AUTH_STATUS_PROVIDER_H_
#define CHROME_BROWSER_SIGNIN_FAKE_AUTH_STATUS_PROVIDER_H_

#include "chrome/browser/signin/signin_global_error.h"

class FakeAuthStatusProvider : public SigninGlobalError::AuthStatusProvider {
 public:
  explicit FakeAuthStatusProvider(SigninGlobalError* error);
  virtual ~FakeAuthStatusProvider();

  
  
  void SetAuthError(const std::string& account_id,
                    const GoogleServiceAuthError& error);

  void set_error_without_status_change(const GoogleServiceAuthError& error) {
    auth_error_ = error;
  }

  
  virtual std::string GetAccountId() const OVERRIDE;
  virtual GoogleServiceAuthError GetAuthStatus() const OVERRIDE;

 private:
  SigninGlobalError* global_error_;
  std::string account_id_;
  GoogleServiceAuthError auth_error_;
};

#endif  

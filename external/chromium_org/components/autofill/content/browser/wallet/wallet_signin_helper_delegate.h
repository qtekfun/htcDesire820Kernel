// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_WALLET_SIGNIN_HELPER_DELEGATE_H_
#define CHROME_BROWSER_UI_AUTOFILL_WALLET_SIGNIN_HELPER_DELEGATE_H_

#include <string>
#include <vector>

class GoogleServiceAuthError;

namespace autofill {
namespace wallet {

class WalletSigninHelperDelegate {
 public:
  virtual ~WalletSigninHelperDelegate() {}

  
  virtual void OnPassiveSigninSuccess() = 0;

  
  virtual void OnPassiveSigninFailure(const GoogleServiceAuthError& error) = 0;

  
  virtual void OnDidFetchWalletCookieValue(const std::string& cookie_value) = 0;
};

}  
}  

#endif  

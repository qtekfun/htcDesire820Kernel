// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CONTENT_BROWSER_WALLET_WALLET_SIGNIN_HELPER_H_
#define COMPONENTS_AUTOFILL_CONTENT_BROWSER_WALLET_WALLET_SIGNIN_HELPER_H_

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "net/url_request/url_fetcher_delegate.h"

namespace net {
class URLFetcher;
class URLRequestContextGetter;
class URLRequestStatus;
}

class GoogleServiceAuthError;

namespace autofill {
namespace wallet {

class WalletSigninHelperDelegate;

class WalletSigninHelper : public net::URLFetcherDelegate {
 public:
  
  
  
  WalletSigninHelper(WalletSigninHelperDelegate* delegate,
                     net::URLRequestContextGetter* getter);

  virtual ~WalletSigninHelper();

  
  
  
  
  
  
  
  void StartPassiveSignin(size_t user_index);

  
  void StartWalletCookieValueFetch();

 private:
  
  void OnServiceError(const GoogleServiceAuthError& error);

  
  void OnOtherError();

  
  virtual void OnURLFetchComplete(const net::URLFetcher* fetcher) OVERRIDE;

  
  void ReturnWalletCookieValue(const std::string& cookie_value);

  
  WalletSigninHelperDelegate* const delegate_;

  
  net::URLRequestContextGetter* const getter_;

  
  scoped_ptr<net::URLFetcher> url_fetcher_;

  base::WeakPtrFactory<WalletSigninHelper> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(WalletSigninHelper);
};

}  
}  

#endif  

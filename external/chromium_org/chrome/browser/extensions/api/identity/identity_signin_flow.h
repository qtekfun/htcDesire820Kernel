// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_IDENTITY_IDENTITY_SIGNIN_FLOW_H_
#define CHROME_BROWSER_EXTENSIONS_API_IDENTITY_IDENTITY_SIGNIN_FLOW_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "google_apis/gaia/oauth2_token_service.h"

class Profile;

namespace extensions {

class IdentitySigninFlow : public OAuth2TokenService::Observer {
 public:
  class Delegate {
   public:
    Delegate() {}
    virtual ~Delegate() {}
    
    virtual void SigninSuccess() = 0;
    
    virtual void SigninFailed() = 0;

    DISALLOW_COPY_AND_ASSIGN(Delegate);
  };

  IdentitySigninFlow(Delegate* delegate,
                     Profile* profile);
  virtual ~IdentitySigninFlow();

  
  void Start();

  
  virtual void OnRefreshTokenAvailable(const std::string& account_id) OVERRIDE;

 private:
  Delegate* delegate_;
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(IdentitySigninFlow);
};

}  

#endif  

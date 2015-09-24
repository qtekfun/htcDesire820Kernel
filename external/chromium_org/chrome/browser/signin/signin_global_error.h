// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SIGNIN_SIGNIN_GLOBAL_ERROR_H_
#define CHROME_BROWSER_SIGNIN_SIGNIN_GLOBAL_ERROR_H_

#include <set>
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/ui/global_error/global_error.h"
#include "google_apis/gaia/google_service_auth_error.h"

class Profile;

class SigninGlobalError : public GlobalErrorWithStandardBubble {
 public:
  class AuthStatusProvider {
   public:
    AuthStatusProvider();
    virtual ~AuthStatusProvider();

    
    virtual std::string GetAccountId() const = 0;

    
    
    virtual GoogleServiceAuthError GetAuthStatus() const = 0;
  };

  explicit SigninGlobalError(Profile* profile);
  virtual ~SigninGlobalError();

  
  
  void AddProvider(const AuthStatusProvider* provider);

  
  
  void RemoveProvider(const AuthStatusProvider* provider);

  
  void AuthStatusChanged();

  std::string GetAccountIdOfLastAuthError() const { return account_id_; }

  GoogleServiceAuthError GetLastAuthError() const { return auth_error_; }

  
  virtual bool HasMenuItem() OVERRIDE;
  virtual int MenuItemCommandID() OVERRIDE;
  virtual base::string16 MenuItemLabel() OVERRIDE;
  virtual void ExecuteMenuItem(Browser* browser) OVERRIDE;
  virtual bool HasBubbleView() OVERRIDE;
  virtual base::string16 GetBubbleViewTitle() OVERRIDE;
  virtual std::vector<base::string16> GetBubbleViewMessages() OVERRIDE;
  virtual base::string16 GetBubbleViewAcceptButtonLabel() OVERRIDE;
  virtual base::string16 GetBubbleViewCancelButtonLabel() OVERRIDE;
  virtual void OnBubbleViewDidClose(Browser* browser) OVERRIDE;
  virtual void BubbleViewAcceptButtonPressed(Browser* browser) OVERRIDE;
  virtual void BubbleViewCancelButtonPressed(Browser* browser) OVERRIDE;

  
  static SigninGlobalError* GetForProfile(Profile* profile);

 private:
  std::set<const AuthStatusProvider*> provider_set_;

  
  std::string account_id_;

  
  
  GoogleServiceAuthError auth_error_;

  
  Profile* profile_;
};

#endif  

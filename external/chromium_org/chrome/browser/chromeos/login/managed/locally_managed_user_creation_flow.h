// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_MANAGED_LOCALLY_MANAGED_USER_CREATION_FLOW_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_MANAGED_LOCALLY_MANAGED_USER_CREATION_FLOW_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "chrome/browser/chromeos/login/user_flow.h"

class Profile;

namespace chromeos {

class LocallyManagedUserCreationFlow : public ExtendedUserFlow {
 public:
  explicit LocallyManagedUserCreationFlow(const std::string& manager_id);
  virtual ~LocallyManagedUserCreationFlow();

  virtual bool ShouldShowSettings() OVERRIDE;
  virtual bool ShouldLaunchBrowser() OVERRIDE;
  virtual bool ShouldSkipPostLoginScreens() OVERRIDE;
  virtual bool HandleLoginFailure(const LoginFailure& failure) OVERRIDE;
  virtual bool HandlePasswordChangeDetected() OVERRIDE;
  virtual void HandleOAuthTokenStatusChange(User::OAuthTokenStatus status)
      OVERRIDE;
  virtual void LaunchExtraSteps(Profile* profile) OVERRIDE;
 private:
  
  base::string16 name_;
  
  std::string password_;

  
  bool token_validated_;

  
  
  bool logged_in_;

  Profile* manager_profile_;

  DISALLOW_COPY_AND_ASSIGN(LocallyManagedUserCreationFlow);
};

}  

#endif  

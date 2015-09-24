// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_ACCOUNT_CREATION_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_ACCOUNT_CREATION_VIEW_H_
#pragma once

#include <string>

#include "chrome/browser/chromeos/login/web_page_view.h"
#include "views/view.h"

class Profile;
class SiteContents;

namespace chromeos {

class AccountCreationViewDelegate {
 public:
  virtual ~AccountCreationViewDelegate() {}

  
  
  
  virtual void OnUserCreated(const std::string& username,
                             const std::string& password) = 0;
};

class AccountCreationDomView : public WebPageDomView {
 public:
  AccountCreationDomView();
  virtual ~AccountCreationDomView();

  
  void SetAccountCreationViewDelegate(AccountCreationViewDelegate* delegate);

 protected:
  
  virtual TabContents* CreateTabContents(Profile* profile,
                                         SiteInstance* instance);

 private:
  AccountCreationViewDelegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(AccountCreationDomView);
};

class AccountCreationView : public WebPageView {
 public:
  AccountCreationView();
  virtual ~AccountCreationView();

  
  void SetAccountCreationViewDelegate(AccountCreationViewDelegate* delegate);

 protected:
  virtual WebPageDomView* dom_view() { return dom_view_; }

 private:
  
  AccountCreationDomView* dom_view_;

  DISALLOW_COPY_AND_ASSIGN(AccountCreationView);
};

}  

#endif  

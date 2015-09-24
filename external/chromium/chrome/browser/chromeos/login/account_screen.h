// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_ACCOUNT_SCREEN_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_ACCOUNT_SCREEN_H_
#pragma once

#include <string>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/chromeos/login/account_creation_view.h"
#include "chrome/browser/chromeos/login/screen_observer.h"
#include "chrome/browser/chromeos/login/view_screen.h"
#include "chrome/browser/chromeos/login/web_page_screen.h"
#include "chrome/browser/chromeos/login/web_page_view.h"
#include "content/browser/tab_contents/tab_contents_delegate.h"

class GURL;
class WizardScreenDelegate;

namespace chromeos {

class AccountScreen : public ViewScreen<AccountCreationView>,
                      public WebPageScreen,
                      public WebPageDelegate,
                      public AccountCreationViewDelegate {
 public:
  explicit AccountScreen(WizardScreenDelegate* delegate);
  virtual ~AccountScreen();

  
  virtual void OnPageLoaded();
  virtual void OnPageLoadFailed(const std::string& url);

  
  virtual void OnUserCreated(const std::string& username,
                             const std::string& password);

  
  static void set_new_account_page_url(const GURL& url);
  
  static void set_check_for_https(bool check) { check_for_https_ = check; }

 private:
  
  virtual void CreateView();
  virtual void Refresh();
  virtual AccountCreationView* AllocateView();

  
  virtual void NavigationStateChanged(const TabContents* source,
                                      unsigned changed_flags);
  virtual void LoadingStateChanged(TabContents* source);
  virtual void HandleKeyboardEvent(const NativeWebKeyboardEvent& event);

  
  virtual void CloseScreen(ScreenObserver::ExitCodes code);

  
  static scoped_ptr<GURL> new_account_page_url_;
  
  static bool check_for_https_;

  DISALLOW_COPY_AND_ASSIGN(AccountScreen);
};

}  

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_REGISTRATION_SCREEN_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_REGISTRATION_SCREEN_H_
#pragma once

#include <string>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/chromeos/login/screen_observer.h"
#include "chrome/browser/chromeos/login/view_screen.h"
#include "chrome/browser/chromeos/login/web_page_screen.h"
#include "chrome/browser/chromeos/login/web_page_view.h"

namespace net {
class URLRequest;
class URLRequestJob;
}  

class GURL;
class Profile;
class SiteContents;
class WizardScreenDelegate;

namespace chromeos {

class RegistrationDomView : public WebPageDomView {
 public:
  RegistrationDomView() {}

 protected:
  
  virtual TabContents* CreateTabContents(Profile* profile,
                                         SiteInstance* instance) {
    return new WizardWebPageViewTabContents(profile,
                                            instance,
                                            page_delegate_);
  }

  DISALLOW_COPY_AND_ASSIGN(RegistrationDomView);
};

class RegistrationView : public WebPageView {
 public:
  RegistrationView() : dom_view_(new RegistrationDomView()) {}

 protected:
  virtual WebPageDomView* dom_view() { return dom_view_; }

 private:
  
  RegistrationDomView* dom_view_;

  DISALLOW_COPY_AND_ASSIGN(RegistrationView);
};

class RegistrationScreen : public ViewScreen<RegistrationView>,
                           public WebPageScreen,
                           public WebPageDelegate {
 public:
  explicit RegistrationScreen(WizardScreenDelegate* delegate);

  
  virtual void OnPageLoaded();
  virtual void OnPageLoadFailed(const std::string& url);

  
  static void set_registration_host_page_url(const GURL& url);

  
  static net::URLRequestJob* Factory(net::URLRequest* request,
                                     const std::string& scheme);

 private:
  
  virtual void CreateView();
  virtual void Refresh();
  virtual RegistrationView* AllocateView();

  
  virtual void LoadingStateChanged(TabContents* source) {}
  virtual void NavigationStateChanged(const TabContents* source,
                                      unsigned changed_flags) {}
  virtual void OpenURLFromTab(TabContents* source,
                              const GURL& url,
                              const GURL& referrer,
                              WindowOpenDisposition disposition,
                              PageTransition::Type transition);
  virtual void HandleKeyboardEvent(const NativeWebKeyboardEvent& event);

  
  virtual void CloseScreen(ScreenObserver::ExitCodes code);

  
  static scoped_ptr<GURL> host_page_url_;

  DISALLOW_COPY_AND_ASSIGN(RegistrationScreen);
};

}  

#endif  

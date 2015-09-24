// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_HTML_PAGE_SCREEN_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_HTML_PAGE_SCREEN_H_
#pragma once

#include <string>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/chromeos/login/screen_observer.h"
#include "chrome/browser/chromeos/login/view_screen.h"
#include "chrome/browser/chromeos/login/web_page_screen.h"
#include "chrome/browser/chromeos/login/web_page_view.h"

class WizardScreenDelegate;

namespace chromeos {

class HTMLPageDomView : public WebPageDomView {
 public:
  HTMLPageDomView() {}

 protected:
  
  virtual TabContents* CreateTabContents(Profile* profile,
                                         SiteInstance* instance);

 private:
  DISALLOW_COPY_AND_ASSIGN(HTMLPageDomView);
};

class HTMLPageView : public WebPageView {
 public:
  HTMLPageView();

 protected:
  virtual WebPageDomView* dom_view() { return dom_view_; }

 private:
  
  HTMLPageDomView* dom_view_;

  DISALLOW_COPY_AND_ASSIGN(HTMLPageView);
};

class HTMLPageScreen : public ViewScreen<HTMLPageView>,
                       public WebPageScreen,
                       public WebPageDelegate {
 public:
  HTMLPageScreen(WizardScreenDelegate* delegate, const std::string& url);

  
  virtual void OnPageLoaded();
  virtual void OnPageLoadFailed(const std::string& url);

 protected:
  
  virtual void OnNetworkTimeout();

 private:
  
  virtual void CreateView();
  virtual void Refresh();
  virtual HTMLPageView* AllocateView();

  virtual void LoadingStateChanged(TabContents* source);
  virtual void NavigationStateChanged(const TabContents* source,
                                      unsigned changed_flags);
  virtual void HandleKeyboardEvent(const NativeWebKeyboardEvent& event);

  
  virtual void CloseScreen(ScreenObserver::ExitCodes code);

  
  std::string url_;

  DISALLOW_COPY_AND_ASSIGN(HTMLPageScreen);
};

}  

#endif  

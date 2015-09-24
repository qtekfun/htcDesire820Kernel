// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_WEB_PAGE_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_WEB_PAGE_VIEW_H_
#pragma once

#include <string>

#include "base/timer.h"
#include "chrome/browser/ui/views/dom_view.h"
#include "content/browser/tab_contents/tab_contents.h"
#include "views/view.h"

class Profile;
class SiteContents;
class TabContentsDelegate;

namespace views {
class Label;
class Throbber;
}  

namespace chromeos {

class WebPageDelegate {
 public:
  virtual ~WebPageDelegate() {}

  
  virtual void OnPageLoaded() = 0;

  
  virtual void OnPageLoadFailed(const std::string& url) = 0;
};

class WizardWebPageViewTabContents : public TabContents {
 public:
  WizardWebPageViewTabContents(Profile* profile,
                               SiteInstance* site_instance,
                               WebPageDelegate* page_delegate);

  virtual void DidFailProvisionalLoadWithError(
      RenderViewHost* render_view_host,
      bool is_main_frame,
      int error_code,
      const GURL& url,
      bool showing_repost_interstitial);

  virtual void DidDisplayInsecureContent();
  virtual void DidRunInsecureContent(const std::string& security_origin);
  virtual void DocumentLoadedInFrame(long long frame_id);
  virtual void DidFinishLoad(long long frame_id);
  virtual void OnContentBlocked(ContentSettingsType type);

 private:
  WebPageDelegate* page_delegate_;

  DISALLOW_COPY_AND_ASSIGN(WizardWebPageViewTabContents);
};

class WebPageDomView : public DOMView {
 public:
  WebPageDomView() : page_delegate_(NULL) {}

  
  void SetTabContentsDelegate(TabContentsDelegate* delegate);

  
  void set_web_page_delegate(WebPageDelegate* delegate) {
    page_delegate_ = delegate;
  }

 protected:
  
  virtual TabContents* CreateTabContents(Profile* profile,
                                         SiteInstance* instance) = 0;
  WebPageDelegate* page_delegate_;

 private:
  DISALLOW_COPY_AND_ASSIGN(WebPageDomView);
};

class WebPageView : public views::View {
 public:
  WebPageView() : throbber_(NULL), connecting_label_(NULL) {}

  
  void Init();

  
  
  void InitDOM(Profile* profile, SiteInstance* site_instance);

  
  
  void LoadURL(const GURL& url);

  
  void SetTabContentsDelegate(TabContentsDelegate* delegate);

  
  void SetWebPageDelegate(WebPageDelegate* delegate);

  
  void ShowPageContent();

 protected:
  virtual WebPageDomView* dom_view() = 0;

 private:
  
  virtual void Layout();

  
  void ShowRenderedPage();

  
  void ShowWaitingControls();

  
  views::Throbber* throbber_;

  
  views::Label* connecting_label_;

  
  base::OneShotTimer<WebPageView> start_timer_;

  
  base::OneShotTimer<WebPageView> stop_timer_;

  DISALLOW_COPY_AND_ASSIGN(WebPageView);
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_WEBSITE_SETTINGS_WEBSITE_SETTINGS_POPUP_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_WEBSITE_SETTINGS_WEBSITE_SETTINGS_POPUP_VIEW_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/ui/views/website_settings/permission_selector_view_observer.h"
#include "chrome/browser/ui/website_settings/website_settings_ui.h"
#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/link_listener.h"
#include "ui/views/controls/tabbed_pane/tabbed_pane_listener.h"

class Browser;
class GURL;
class PermissionSelectorView;
class PopupHeaderView;
class Profile;

namespace content {
struct SSLStatus;
class WebContents;
}

namespace views {
class Link;
class TabbedPane;
class Widget;
}

class WebsiteSettingsPopupView
    : public PermissionSelectorViewObserver,
      public views::BubbleDelegateView,
      public views::ButtonListener,
      public views::LinkListener,
      public views::TabbedPaneListener,
      public WebsiteSettingsUI {
 public:
  virtual ~WebsiteSettingsPopupView();

  static void ShowPopup(views::View* anchor_view,
                        Profile* profile,
                        content::WebContents* web_contents,
                        const GURL& url,
                        const content::SSLStatus& ssl,
                        Browser* browser);

 private:
  WebsiteSettingsPopupView(views::View* anchor_view,
                           Profile* profile,
                           content::WebContents* web_contents,
                           const GURL& url,
                           const content::SSLStatus& ssl,
                           Browser* browser);

  
  virtual void OnPermissionChanged(
      PermissionSelectorView* selector) OVERRIDE;

  
  virtual void OnWidgetDestroying(views::Widget* widget) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* button,
                             const ui::Event& event) OVERRIDE;

  
  virtual void LinkClicked(views::Link* source, int event_flags) OVERRIDE;

  
  virtual void TabSelectedAt(int index) OVERRIDE;

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;

  
  virtual void SetCookieInfo(const CookieInfoList& cookie_info_list) OVERRIDE;
  virtual void SetPermissionInfo(
      const PermissionInfoList& permission_info_list) OVERRIDE;
  virtual void SetIdentityInfo(const IdentityInfo& identity_info) OVERRIDE;
  virtual void SetFirstVisit(const base::string16& first_visit) OVERRIDE;
  virtual void SetSelectedTab(TabId tab_id) OVERRIDE;

  
  
  views::View* CreatePermissionsTab() WARN_UNUSED_RESULT;

  
  
  views::View* CreateConnectionTab() WARN_UNUSED_RESULT;

  
  
  
  
  views::View* CreateSection(const base::string16& headline,
                             views::View* contents,
                             views::Link* link) WARN_UNUSED_RESULT;

  
  
  
  
  
  
  void ResetConnectionSection(views::View* section_container,
                              const gfx::Image& icon,
                              const base::string16& headline,
                              const base::string16& text,
                              views::Link* link);
  
  void HandleLinkClickedAsync(views::Link* source);

  
  
  content::WebContents* web_contents_;

  
  Browser* browser_;

  
  scoped_ptr<WebsiteSettings> presenter_;

  PopupHeaderView* header_;  

  
  views::TabbedPane* tabbed_pane_;

  
  
  views::View* site_data_content_;
  
  views::Link* cookie_dialog_link_;
  
  
  views::View* permissions_content_;

  
  views::View* connection_tab_;
  
  
  views::View* identity_info_content_;
  
  
  
  views::Link* certificate_dialog_link_;
  
  
  int cert_id_;

  
  
  views::Link* help_center_link_;

  views::View* connection_info_content_;
  views::View* page_info_content_;

  base::WeakPtrFactory<WebsiteSettingsPopupView> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(WebsiteSettingsPopupView);
};

#endif  

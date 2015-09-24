// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_WEBSITE_SETTINGS_WEBSITE_SETTINGS_POPUP_GTK_H_
#define CHROME_BROWSER_UI_GTK_WEBSITE_SETTINGS_WEBSITE_SETTINGS_POPUP_GTK_H_

#include <gtk/gtk.h>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "chrome/browser/ui/gtk/bubble/bubble_gtk.h"
#include "chrome/browser/ui/gtk/website_settings/permission_selector_observer.h"
#include "chrome/browser/ui/website_settings/website_settings_ui.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class Browser;
class CustomDrawButton;
class GtkThemeService;
class GURL;
class PermissionSelector;
class Profile;
class WebsiteSettings;

namespace content {
struct SSLStatus;
class WebContents;
}

class WebsiteSettingsPopupGtk : public WebsiteSettingsUI,
                                public PermissionSelectorObserver,
                                public BubbleDelegateGtk,
                                public content::NotificationObserver {
 public:
  
  
  
  
  
  
  static void Show(gfx::NativeWindow parent,
                   Profile* profile,
                   content::WebContents* web_contents,
                   const GURL& url,
                   const content::SSLStatus& ssl);

  virtual ~WebsiteSettingsPopupGtk();

 private:
  WebsiteSettingsPopupGtk(gfx::NativeWindow parent,
                          Profile* profile,
                          content::WebContents* web_contents,
                          const GURL& url,
                          const content::SSLStatus& ssl);

  
  virtual void SetCookieInfo(const CookieInfoList& cookie_info_list) OVERRIDE;
  virtual void SetPermissionInfo(
      const PermissionInfoList& permission_info_list) OVERRIDE;
  virtual void SetIdentityInfo(const IdentityInfo& identity_info) OVERRIDE;
  virtual void SetFirstVisit(const base::string16& first_visit) OVERRIDE;
  virtual void SetSelectedTab(WebsiteSettingsUI::TabId tab_id) OVERRIDE;

  
  virtual void OnPermissionChanged(PermissionSelector* selector) OVERRIDE;

  
  virtual void BubbleClosing(BubbleGtk* bubble, bool closed_by_escape) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void InitContents();

  
  
  GtkWidget* BuildTab(int ids);

  
  int TabstripButtonToTabIndex(GtkWidget* tab);

  
  CHROMEGTK_CALLBACK_1(WebsiteSettingsPopupGtk, gboolean,
                       OnTabButtonPress, GdkEvent*);
  CHROMEGTK_CALLBACK_1(WebsiteSettingsPopupGtk, gboolean,
                       OnTabExpose, GdkEventExpose*);
  CHROMEGTK_CALLBACK_1(WebsiteSettingsPopupGtk, gboolean,
                       OnTabstripExpose, GdkEventExpose*);
  CHROMEGTK_CALLBACK_0(WebsiteSettingsPopupGtk, void, OnCookiesLinkClicked);
  CHROMEGTK_CALLBACK_0(WebsiteSettingsPopupGtk, void, OnViewCertLinkClicked);
  CHROMEGTK_CALLBACK_0(WebsiteSettingsPopupGtk, void, OnCloseButtonClicked);
  CHROMEGTK_CALLBACK_0(WebsiteSettingsPopupGtk, void, OnHelpLinkClicked);

  
  GtkWindow* parent_;

  
  GtkWidget* contents_;

  
  GtkWidget* anchor_;

  
  GtkThemeService* theme_service_;

  
  BubbleGtk* bubble_;

  Profile* profile_;

  content::WebContents* web_contents_;

  
  
  Browser* browser_;

  
  
  int cert_id_;

  
  GtkWidget* header_box_;

  
  scoped_ptr<CustomDrawButton> close_button_;

  
  GtkWidget* cookies_section_contents_;

  
  GtkWidget* permissions_section_contents_;

  
  
  GtkWidget* identity_contents_;

  
  GtkWidget* connection_contents_;

  
  GtkWidget* first_visit_contents_;

  
  GtkWidget* tabstrip_alignment_;

  
  GtkWidget* notebook_;

  
  
  scoped_ptr<WebsiteSettings> presenter_;

  
  
  ScopedVector<PermissionSelector> selectors_;

  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(WebsiteSettingsPopupGtk);
};

#endif  

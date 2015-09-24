// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_THEME_INSTALLED_INFOBAR_DELEGATE_H_
#define CHROME_BROWSER_EXTENSIONS_THEME_INSTALLED_INFOBAR_DELEGATE_H_

#include <string>

#include "base/compiler_specific.h"
#include "chrome/browser/infobars/confirm_infobar_delegate.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class ExtensionService;
class InfoBarService;
class Profile;
class ThemeService;

namespace extensions {
class Extension;
}

class ThemeInstalledInfoBarDelegate : public ConfirmInfoBarDelegate,
                                      public content::NotificationObserver {
 public:
  
  
  static void Create(const extensions::Extension* new_theme,
                     Profile* profile,
                     const std::string& previous_theme_id,
                     bool previous_using_native_theme);

 private:
  ThemeInstalledInfoBarDelegate(ExtensionService* extension_service,
                                ThemeService* theme_service,
                                const extensions::Extension* new_theme,
                                const std::string& previous_theme_id,
                                bool previous_using_native_theme);
  virtual ~ThemeInstalledInfoBarDelegate();

  
  virtual int GetIconID() const OVERRIDE;
  virtual Type GetInfoBarType() const OVERRIDE;
  virtual ThemeInstalledInfoBarDelegate*
      AsThemePreviewInfobarDelegate() OVERRIDE;
  virtual base::string16 GetMessageText() const OVERRIDE;
  virtual int GetButtons() const OVERRIDE;
  virtual base::string16 GetButtonLabel(InfoBarButton button) const OVERRIDE;
  virtual bool Cancel() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  ExtensionService* extension_service_;
  ThemeService* theme_service_;

  
  std::string name_;

  
  std::string theme_id_;

  
  std::string previous_theme_id_;
  bool previous_using_native_theme_;

  
  content::NotificationRegistrar registrar_;
};

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_THEME_INSTALLED_INFOBAR_DELEGATE_H_
#define CHROME_BROWSER_EXTENSIONS_THEME_INSTALLED_INFOBAR_DELEGATE_H_
#pragma once

#include "chrome/browser/tab_contents/confirm_infobar_delegate.h"
#include "content/common/notification_registrar.h"

class ThemeService;
class Extension;
class SkBitmap;
class TabContents;

class ThemeInstalledInfoBarDelegate : public ConfirmInfoBarDelegate,
                                      public NotificationObserver {
 public:
  ThemeInstalledInfoBarDelegate(TabContents* tab_contents,
                                const Extension* new_theme,
                                const std::string& previous_theme_id);

  
  
  bool MatchesTheme(const Extension* theme) const;

 protected:
  virtual ~ThemeInstalledInfoBarDelegate();

  ThemeService* theme_service() { return theme_service_; }

  
  virtual bool Cancel();

 private:
  
  virtual void InfoBarClosed();
  virtual SkBitmap* GetIcon() const;
  virtual ThemeInstalledInfoBarDelegate* AsThemePreviewInfobarDelegate();
  virtual string16 GetMessageText() const;
  virtual int GetButtons() const;
  virtual string16 GetButtonLabel(InfoBarButton button) const;

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  Profile* profile_;
  ThemeService* theme_service_;

  
  std::string name_;

  
  std::string theme_id_;

  
  std::string previous_theme_id_;

  
  TabContents* tab_contents_;

  
  NotificationRegistrar registrar_;
};

#endif  

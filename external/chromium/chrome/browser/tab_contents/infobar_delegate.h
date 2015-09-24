// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CONTENTS_INFOBAR_DELEGATE_H_
#define CHROME_BROWSER_TAB_CONTENTS_INFOBAR_DELEGATE_H_
#pragma once

#include "base/basictypes.h"
#include "base/string16.h"
#include "content/browser/tab_contents/navigation_controller.h"
#include "webkit/glue/window_open_disposition.h"

class ConfirmInfoBarDelegate;
class ExtensionInfoBarDelegate;
class InfoBar;
class LinkInfoBarDelegate;
class PluginInstallerInfoBarDelegate;
class SkBitmap;
class ThemeInstalledInfoBarDelegate;
class TranslateInfoBarDelegate;

class InfoBarDelegate {
 public:
  
  
  enum Type {
    WARNING_TYPE,
    PAGE_ACTION_TYPE,
  };

  virtual ~InfoBarDelegate();

  
  
  virtual InfoBar* CreateInfoBar() = 0;

  
  
  
  
  
  
  virtual bool EqualsDelegate(InfoBarDelegate* delegate) const;

  
  
  
  virtual bool ShouldExpire(
      const NavigationController::LoadCommittedDetails& details) const;

  
  virtual void InfoBarDismissed();

  
  
  virtual void InfoBarClosed();

  
  
  virtual SkBitmap* GetIcon() const;

  
  
  virtual Type GetInfoBarType() const;

  
  virtual ConfirmInfoBarDelegate* AsConfirmInfoBarDelegate();
  virtual ExtensionInfoBarDelegate* AsExtensionInfoBarDelegate();
  virtual LinkInfoBarDelegate* AsLinkInfoBarDelegate();
  virtual PluginInstallerInfoBarDelegate* AsPluginInstallerInfoBarDelegate();
  virtual ThemeInstalledInfoBarDelegate* AsThemePreviewInfobarDelegate();
  virtual TranslateInfoBarDelegate* AsTranslateInfoBarDelegate();

 protected:
  
  
  
  explicit InfoBarDelegate(TabContents* contents);

  
  
  
  void StoreActiveEntryUniqueID(TabContents* contents);

 private:
  
  
  int contents_unique_id_;

  DISALLOW_COPY_AND_ASSIGN(InfoBarDelegate);
};

#endif  

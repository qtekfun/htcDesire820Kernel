// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INFOBARS_INFOBAR_DELEGATE_H_
#define CHROME_BROWSER_INFOBARS_INFOBAR_DELEGATE_H_

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "chrome/browser/infobars/infobar_service.h"
#include "ui/base/window_open_disposition.h"

class AutoLoginInfoBarDelegate;
class ConfirmInfoBarDelegate;
class ExtensionInfoBarDelegate;
class InfoBar;
class InsecureContentInfoBarDelegate;
class MediaStreamInfoBarDelegate;
class PopupBlockedInfoBarDelegate;
class RegisterProtocolHandlerInfoBarDelegate;
class ScreenCaptureInfoBarDelegate;
class ThemeInstalledInfoBarDelegate;
class ThreeDAPIInfoBarDelegate;
class TranslateInfoBarDelegate;

namespace gfx {
class Image;
}

class InfoBarDelegate {
 public:
  
  
  enum Type {
    WARNING_TYPE,
    PAGE_ACTION_TYPE,
  };

  enum InfoBarAutomationType {
    CONFIRM_INFOBAR,
    PASSWORD_INFOBAR,
    RPH_INFOBAR,
    UNKNOWN_INFOBAR,
  };

  
  static const int kNoIconID;

  
  
  virtual ~InfoBarDelegate();

  virtual InfoBarAutomationType GetInfoBarAutomationType() const;

  
  
  
  
  
  
  virtual bool EqualsDelegate(InfoBarDelegate* delegate) const;

  
  
  
  
  
  virtual bool ShouldExpire(const content::LoadCommittedDetails& details) const;

  
  virtual void InfoBarDismissed();

  
  
  virtual int GetIconID() const;

  
  
  virtual Type GetInfoBarType() const;

  
  virtual AutoLoginInfoBarDelegate* AsAutoLoginInfoBarDelegate();
  virtual ConfirmInfoBarDelegate* AsConfirmInfoBarDelegate();
  virtual ExtensionInfoBarDelegate* AsExtensionInfoBarDelegate();
  virtual InsecureContentInfoBarDelegate* AsInsecureContentInfoBarDelegate();
  virtual MediaStreamInfoBarDelegate* AsMediaStreamInfoBarDelegate();
  virtual PopupBlockedInfoBarDelegate* AsPopupBlockedInfoBarDelegate();
  virtual RegisterProtocolHandlerInfoBarDelegate*
      AsRegisterProtocolHandlerInfoBarDelegate();
  virtual ScreenCaptureInfoBarDelegate* AsScreenCaptureInfoBarDelegate();
  virtual ThemeInstalledInfoBarDelegate* AsThemePreviewInfobarDelegate();
  virtual ThreeDAPIInfoBarDelegate* AsThreeDAPIInfoBarDelegate();
  virtual TranslateInfoBarDelegate* AsTranslateInfoBarDelegate();

  void set_infobar(InfoBar* infobar) { infobar_ = infobar; }

  
  
  
  void StoreActiveEntryUniqueID();

  
  
  virtual gfx::Image GetIcon() const;

  
  
  content::WebContents* web_contents();

 protected:
  InfoBarDelegate();

  
  virtual bool ShouldExpireInternal(
      const content::LoadCommittedDetails& details) const;

  int contents_unique_id() const { return contents_unique_id_; }
  InfoBar* infobar() { return infobar_; }

 private:
  
  
  int contents_unique_id_;

  
  InfoBar* infobar_;

  DISALLOW_COPY_AND_ASSIGN(InfoBarDelegate);
};

#endif  

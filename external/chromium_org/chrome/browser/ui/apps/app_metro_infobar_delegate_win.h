// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APPS_APP_METRO_INFOBAR_DELEGATE_WIN_H_
#define CHROME_BROWSER_UI_APPS_APP_METRO_INFOBAR_DELEGATE_WIN_H_

#include <string>

#include "chrome/browser/infobars/confirm_infobar_delegate.h"

class InfoBarService;
class Profile;

class AppMetroInfoBarDelegateWin : public ConfirmInfoBarDelegate {
 public:
  enum Mode {
    SHOW_APP_LIST,
    LAUNCH_PACKAGED_APP
  };

  
  
  static void Create(Profile* profile,
                     Mode mode,
                     const std::string& extension_id);

 private:
  AppMetroInfoBarDelegateWin(Mode mode, const std::string& extension_id);
  virtual ~AppMetroInfoBarDelegateWin();

  
  virtual int GetIconID() const OVERRIDE;
  virtual base::string16 GetMessageText() const OVERRIDE;
  virtual base::string16 GetButtonLabel(InfoBarButton button) const OVERRIDE;
  virtual bool Accept() OVERRIDE;
  virtual bool Cancel() OVERRIDE;
  virtual base::string16 GetLinkText() const OVERRIDE;
  virtual bool LinkClicked(WindowOpenDisposition disposition) OVERRIDE;

  Mode mode_;
  std::string extension_id_;

  DISALLOW_COPY_AND_ASSIGN(AppMetroInfoBarDelegateWin);
};

#endif  

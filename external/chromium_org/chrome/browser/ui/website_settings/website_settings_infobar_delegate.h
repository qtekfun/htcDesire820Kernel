// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBSITE_SETTINGS_WEBSITE_SETTINGS_INFOBAR_DELEGATE_H_
#define CHROME_BROWSER_UI_WEBSITE_SETTINGS_WEBSITE_SETTINGS_INFOBAR_DELEGATE_H_

#include "chrome/browser/infobars/confirm_infobar_delegate.h"

class InfoBarService;

class WebsiteSettingsInfoBarDelegate : public ConfirmInfoBarDelegate {
 public:
  
  
  static void Create(InfoBarService* infobar_service);

 private:
  WebsiteSettingsInfoBarDelegate();
  virtual ~WebsiteSettingsInfoBarDelegate();

  
  virtual int GetIconID() const OVERRIDE;
  virtual Type GetInfoBarType() const OVERRIDE;
  virtual base::string16 GetMessageText() const OVERRIDE;
  virtual int GetButtons() const OVERRIDE;
  virtual base::string16 GetButtonLabel(InfoBarButton button) const OVERRIDE;
  virtual bool Accept() OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(WebsiteSettingsInfoBarDelegate);
};

#endif  

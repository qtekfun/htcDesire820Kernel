// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INFOBARS_CONFIRM_INFOBAR_DELEGATE_H_
#define CHROME_BROWSER_INFOBARS_CONFIRM_INFOBAR_DELEGATE_H_

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/infobars/infobar_delegate.h"

class InfoBar;

class ConfirmInfoBarDelegate : public InfoBarDelegate {
 public:
  enum InfoBarButton {
    BUTTON_NONE   = 0,
    BUTTON_OK     = 1 << 0,
    BUTTON_CANCEL = 1 << 1,
  };

  virtual ~ConfirmInfoBarDelegate();

  
  virtual InfoBarAutomationType GetInfoBarAutomationType() const OVERRIDE;

  
  virtual base::string16 GetMessageText() const = 0;

  
  virtual int GetButtons() const;

  
  
  virtual base::string16 GetButtonLabel(InfoBarButton button) const;

  
  virtual bool NeedElevation(InfoBarButton button) const;

  
  
  
  virtual bool Accept();

  
  
  
  virtual bool Cancel();

  
  
  virtual base::string16 GetLinkText() const;

  
  
  
  
  
  virtual bool LinkClicked(WindowOpenDisposition disposition);

 protected:
  ConfirmInfoBarDelegate();

  
  static scoped_ptr<InfoBar> CreateInfoBar(
      scoped_ptr<ConfirmInfoBarDelegate> delegate);

  virtual bool ShouldExpireInternal(
      const content::LoadCommittedDetails& details) const OVERRIDE;

 private:
  
  virtual bool EqualsDelegate(InfoBarDelegate* delegate) const OVERRIDE;
  virtual ConfirmInfoBarDelegate* AsConfirmInfoBarDelegate() OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(ConfirmInfoBarDelegate);
};

#endif  

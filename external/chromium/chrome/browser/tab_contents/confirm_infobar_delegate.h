// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CONTENTS_CONFIRM_INFOBAR_DELEGATE_H_
#define CHROME_BROWSER_TAB_CONTENTS_CONFIRM_INFOBAR_DELEGATE_H_
#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/string16.h"
#include "chrome/browser/tab_contents/infobar_delegate.h"

class ConfirmInfoBarDelegate : public InfoBarDelegate {
 public:
  enum InfoBarButton {
    BUTTON_NONE   = 0,
    BUTTON_OK     = 1 << 0,
    BUTTON_CANCEL = 1 << 1,
  };

  
  virtual string16 GetMessageText() const = 0;

  
  virtual int GetButtons() const;

  
  
  virtual string16 GetButtonLabel(InfoBarButton button) const;

  
  virtual bool NeedElevation(InfoBarButton button) const;

  
  
  virtual bool Accept();

  
  
  virtual bool Cancel();

  
  
  virtual string16 GetLinkText();

  
  
  
  
  
  
  virtual bool LinkClicked(WindowOpenDisposition disposition);

 protected:
  explicit ConfirmInfoBarDelegate(TabContents* contents);
  virtual ~ConfirmInfoBarDelegate();

 private:
  
  virtual InfoBar* CreateInfoBar() OVERRIDE;
  virtual bool EqualsDelegate(InfoBarDelegate* delegate) const OVERRIDE;
  virtual ConfirmInfoBarDelegate* AsConfirmInfoBarDelegate() OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(ConfirmInfoBarDelegate);
};

#endif  

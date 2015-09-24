// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INFOBARS_INSECURE_CONTENT_INFOBAR_DELEGATE_H_
#define CHROME_BROWSER_INFOBARS_INSECURE_CONTENT_INFOBAR_DELEGATE_H_

#include "chrome/browser/infobars/confirm_infobar_delegate.h"

class InsecureContentInfoBarDelegate : public ConfirmInfoBarDelegate {
 public:
  enum InfoBarType {
    DISPLAY,  
    RUN,      
  };

  
  
  
  
  static void Create(InfoBarService* infobar_service, InfoBarType type);

 private:
  enum HistogramEvents {
    DISPLAY_INFOBAR_SHOWN = 0,  
    DISPLAY_USER_OVERRIDE,      
    DISPLAY_USER_DID_NOT_LOAD,  
    DISPLAY_INFOBAR_DISMISSED,  
    RUN_INFOBAR_SHOWN,
    RUN_USER_OVERRIDE,
    RUN_USER_DID_NOT_LOAD,
    RUN_INFOBAR_DISMISSED,
    NUM_EVENTS
  };

  explicit InsecureContentInfoBarDelegate(InfoBarType type);
  virtual ~InsecureContentInfoBarDelegate();

  
  virtual void InfoBarDismissed() OVERRIDE;
  virtual InsecureContentInfoBarDelegate*
      AsInsecureContentInfoBarDelegate() OVERRIDE;
  virtual base::string16 GetMessageText() const OVERRIDE;
  virtual base::string16 GetButtonLabel(InfoBarButton button) const OVERRIDE;
  virtual bool Accept() OVERRIDE;
  virtual bool Cancel() OVERRIDE;
  virtual base::string16 GetLinkText() const OVERRIDE;
  virtual bool LinkClicked(WindowOpenDisposition disposition) OVERRIDE;

  InfoBarType type_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(InsecureContentInfoBarDelegate);
};

#endif  


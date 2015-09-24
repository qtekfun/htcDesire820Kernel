// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TAB_MODAL_CONFIRM_DIALOG_DELEGATE_H_
#define CHROME_BROWSER_UI_TAB_MODAL_CONFIRM_DIALOG_DELEGATE_H_

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/window_open_disposition.h"

namespace content {
class WebContents;
}

namespace gfx {
class Image;
}

class TabModalConfirmDialogCloseDelegate {
 public:
  TabModalConfirmDialogCloseDelegate() {}
  virtual ~TabModalConfirmDialogCloseDelegate() {}

  virtual void CloseDialog() = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(TabModalConfirmDialogCloseDelegate);
};

class TabModalConfirmDialogDelegate : public content::NotificationObserver {
 public:
  explicit TabModalConfirmDialogDelegate(content::WebContents* web_contents);
  virtual ~TabModalConfirmDialogDelegate();

  void set_close_delegate(TabModalConfirmDialogCloseDelegate* close_delegate) {
    close_delegate_ = close_delegate;
  }

  
  
  
  
  void Accept();

  
  
  
  
  void Cancel();

  
  
  
  
  
  
  
  void Close();

  
  
  
  
  void LinkClicked(WindowOpenDisposition disposition);

  
  virtual base::string16 GetTitle() = 0;
  virtual base::string16 GetMessage() = 0;

  
  
  virtual gfx::Image* GetIcon();

  
  
  virtual base::string16 GetAcceptButtonTitle();
  virtual base::string16 GetCancelButtonTitle();

  
  
  virtual base::string16 GetLinkText() const;

  
  
  
  virtual const char* GetAcceptButtonIcon();
  virtual const char* GetCancelButtonIcon();

 protected:
  TabModalConfirmDialogCloseDelegate* close_delegate() {
    return close_delegate_;
  }

  
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  content::NotificationRegistrar registrar_;

 private:
  
  
  

  
  virtual void OnAccepted();
  virtual void OnCanceled();

  
  virtual void OnClosed();

  
  
  
  
  virtual void OnLinkClicked(WindowOpenDisposition disposition);

  
  void CloseDialog();

  TabModalConfirmDialogCloseDelegate* close_delegate_;

  
  
  bool closing_;

  DISALLOW_COPY_AND_ASSIGN(TabModalConfirmDialogDelegate);
};

#endif  

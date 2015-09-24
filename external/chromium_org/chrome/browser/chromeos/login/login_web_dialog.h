// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_LOGIN_WEB_DIALOG_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_LOGIN_WEB_DIALOG_H_

#include <string>

#include "base/compiler_specific.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/size.h"
#include "ui/web_dialogs/web_dialog_delegate.h"
#include "url/gurl.h"

namespace chromeos {

class BubbleFrameView;

class LoginWebDialog : public ui::WebDialogDelegate,
                       public content::NotificationObserver {
 public:
  
  class Delegate {
   public:
    
    virtual void OnDialogClosed();

   protected:
    virtual ~Delegate() {}
  };

  enum Style {
    STYLE_GENERIC, 
    STYLE_BUBBLE   
  };

  LoginWebDialog(Delegate* delegate,
                 gfx::NativeWindow parent_window,
                 const base::string16& title,
                 const GURL& url,
                 Style style);
  virtual ~LoginWebDialog();

  void Show();

  
  void SetDialogSize(int width, int height);

  
  void SetDialogTitle(const base::string16& title);

  void set_url(const GURL& url) { url_ = url; }

  bool is_open() const { return is_open_; }

 protected:
  
  virtual ui::ModalType GetDialogModalType() const OVERRIDE;
  virtual base::string16 GetDialogTitle() const OVERRIDE;
  virtual GURL GetDialogContentURL() const OVERRIDE;
  virtual void GetWebUIMessageHandlers(
      std::vector<content::WebUIMessageHandler*>* handlers) const OVERRIDE;
  virtual void GetDialogSize(gfx::Size* size) const OVERRIDE;
  virtual void GetMinimumDialogSize(gfx::Size* size) const OVERRIDE;
  virtual std::string GetDialogArgs() const OVERRIDE;
  
  virtual void OnDialogClosed(const std::string& json_retval) OVERRIDE;
  virtual void OnCloseContents(
      content::WebContents* source, bool* out_close_dialog) OVERRIDE;
  virtual bool ShouldShowDialogTitle() const OVERRIDE;
  virtual bool HandleContextMenu(
      const content::ContextMenuParams& params) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  
  Delegate* delegate_;

  gfx::NativeWindow parent_window_;
  base::string16 title_;
  GURL url_;
  Style style_;
  content::NotificationRegistrar notification_registrar_;
  bool is_open_;

  
  int width_;
  int height_;

  DISALLOW_COPY_AND_ASSIGN(LoginWebDialog);
};

}  

#endif  

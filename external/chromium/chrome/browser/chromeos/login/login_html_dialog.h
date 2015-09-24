// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_LOGIN_HTML_DIALOG_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_LOGIN_HTML_DIALOG_H_
#pragma once

#include <string>

#include "chrome/browser/ui/webui/html_dialog_ui.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/size.h"

namespace chromeos {

class BubbleFrameView;

class LoginHtmlDialog : public HtmlDialogUIDelegate,
                        public NotificationObserver {
 public:
  
  class Delegate {
  public:
    virtual ~Delegate() {}

    
    virtual void OnDialogClosed() = 0;
  };

  enum Style {
    STYLE_GENERIC, 
    STYLE_BUBBLE   
  };

  LoginHtmlDialog(Delegate* delegate,
                  gfx::NativeWindow parent_window,
                  const std::wstring& title,
                  const GURL& url,
                  Style style);
  ~LoginHtmlDialog();

  
  void Show();

  
  void SetDialogSize(int width, int height);

  void set_url(const GURL& url) { url_ = url; }

  bool is_open() const { return is_open_; }

 protected:
  
  virtual bool IsDialogModal() const { return true; }
  virtual std::wstring GetDialogTitle() const { return title_; }
  virtual GURL GetDialogContentURL() const { return url_; }
  virtual void GetWebUIMessageHandlers(
      std::vector<WebUIMessageHandler*>* handlers) const {}
  virtual void GetDialogSize(gfx::Size* size) const;
  virtual std::string GetDialogArgs() const { return std::string(); }
  virtual void OnDialogClosed(const std::string& json_retval);
  virtual void OnCloseContents(TabContents* source, bool* out_close_dialog);
  virtual bool ShouldShowDialogTitle() const { return true; }

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  
  Delegate* delegate_;

  gfx::NativeWindow parent_window_;
  std::wstring title_;
  GURL url_;
  Style style_;
  NotificationRegistrar notification_registrar_;
  BubbleFrameView* bubble_frame_view_;
  bool is_open_;

  
  int width_;
  int height_;

  DISALLOW_COPY_AND_ASSIGN(LoginHtmlDialog);
};

}  

#endif  

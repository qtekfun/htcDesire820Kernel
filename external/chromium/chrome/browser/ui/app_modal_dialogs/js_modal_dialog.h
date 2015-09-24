// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_MODAL_DIALOGS_JS_MODAL_DIALOG_H_
#define CHROME_BROWSER_UI_APP_MODAL_DIALOGS_JS_MODAL_DIALOG_H_
#pragma once

#include <string>

#include "base/utf_string_conversions.h"
#include "build/build_config.h"
#include "chrome/browser/ui/app_modal_dialogs/app_modal_dialog.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/gfx/native_widget_types.h"

class ExtensionHost;
class NativeAppModalDialog;
class TabContents;

namespace IPC {
class Message;
}

class JavaScriptAppModalDialogDelegate {
 public:
  
  virtual void OnMessageBoxClosed(IPC::Message* reply_msg,
                                  bool success,
                                  const std::wstring& prompt) = 0;

  
  virtual void SetSuppressMessageBoxes(bool suppress_message_boxes) = 0;

  
  virtual gfx::NativeWindow GetMessageBoxRootWindow() = 0;

  
  
  
  
  virtual TabContents* AsTabContents() = 0;
  virtual ExtensionHost* AsExtensionHost() = 0;

 protected:
  virtual ~JavaScriptAppModalDialogDelegate() {}
};

class JavaScriptAppModalDialog : public AppModalDialog,
                                 public NotificationObserver {
 public:
  JavaScriptAppModalDialog(JavaScriptAppModalDialogDelegate* delegate,
                           const std::wstring& title,
                           int dialog_flags,
                           const std::wstring& message_text,
                           const std::wstring& default_prompt_text,
                           bool display_suppress_checkbox,
                           bool is_before_unload_dialog,
                           IPC::Message* reply_msg);
  virtual ~JavaScriptAppModalDialog();

  
  virtual NativeAppModalDialog* CreateNativeDialog();

  JavaScriptAppModalDialogDelegate* delegate() const { return delegate_; }

  
  void OnCancel(bool suppress_js_messages);
  void OnAccept(const std::wstring& prompt_text, bool suppress_js_messages);

  
  
  void OnClose();

  
  int dialog_flags() const { return dialog_flags_; }
  std::wstring message_text() const { return message_text_; }
  std::wstring default_prompt_text() const {
    return UTF16ToWideHack(default_prompt_text_);
  }
  bool display_suppress_checkbox() const { return display_suppress_checkbox_; }
  bool is_before_unload_dialog() const { return is_before_unload_dialog_; }

 private:
  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  void InitNotifications();

  
  void NotifyDelegate(bool success, const std::wstring& prompt_text,
                      bool suppress_js_messages);

  NotificationRegistrar registrar_;

  
  
  JavaScriptAppModalDialogDelegate* delegate_;

  
  
  
  
  ExtensionHost* extension_host_;

  
  int dialog_flags_;
  std::wstring message_text_;
  string16 default_prompt_text_;
  bool display_suppress_checkbox_;
  bool is_before_unload_dialog_;
  IPC::Message* reply_msg_;

  DISALLOW_COPY_AND_ASSIGN(JavaScriptAppModalDialog);
};

#endif  

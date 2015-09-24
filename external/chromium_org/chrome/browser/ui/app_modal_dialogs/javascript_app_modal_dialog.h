// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_MODAL_DIALOGS_JAVASCRIPT_APP_MODAL_DIALOG_H_
#define CHROME_BROWSER_UI_APP_MODAL_DIALOGS_JAVASCRIPT_APP_MODAL_DIALOG_H_

#include <map>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/time/time.h"
#include "chrome/browser/ui/app_modal_dialogs/app_modal_dialog.h"
#include "content/public/browser/javascript_dialog_manager.h"

class ChromeJavaScriptDialogExtraData {
 public:
  ChromeJavaScriptDialogExtraData();

  
  base::TimeTicks last_javascript_message_dismissal_;

  
  bool suppress_javascript_messages_;
};

class JavaScriptAppModalDialog : public AppModalDialog {
 public:
  typedef std::map<void*, ChromeJavaScriptDialogExtraData> ExtraDataMap;

  JavaScriptAppModalDialog(
      content::WebContents* web_contents,
      ExtraDataMap* extra_data_map,
      const base::string16& title,
      content::JavaScriptMessageType javascript_message_type,
      const base::string16& message_text,
      const base::string16& default_prompt_text,
      bool display_suppress_checkbox,
      bool is_before_unload_dialog,
      bool is_reload,
      const content::JavaScriptDialogManager::DialogClosedCallback& callback);
  virtual ~JavaScriptAppModalDialog();

  
  virtual NativeAppModalDialog* CreateNativeDialog() OVERRIDE;
  virtual bool IsJavaScriptModalDialog() OVERRIDE;
  virtual void Invalidate() OVERRIDE;

  
  void OnCancel(bool suppress_js_messages);
  void OnAccept(const base::string16& prompt_text, bool suppress_js_messages);

  
  
  void OnClose();

  
  
  void SetOverridePromptText(const base::string16& prompt_text);

  
  content::JavaScriptMessageType javascript_message_type() const {
    return javascript_message_type_;
  }
  base::string16 message_text() const { return message_text_; }
  base::string16 default_prompt_text() const { return default_prompt_text_; }
  bool display_suppress_checkbox() const { return display_suppress_checkbox_; }
  bool is_before_unload_dialog() const { return is_before_unload_dialog_; }
  bool is_reload() const { return is_reload_; }

 private:
  
  void NotifyDelegate(bool success, const base::string16& prompt_text,
                      bool suppress_js_messages);

  
  
  ExtraDataMap* extra_data_map_;

  
  const content::JavaScriptMessageType javascript_message_type_;
  base::string16 message_text_;
  base::string16 default_prompt_text_;
  bool display_suppress_checkbox_;
  bool is_before_unload_dialog_;
  bool is_reload_;

  content::JavaScriptDialogManager::DialogClosedCallback callback_;

  
  
  base::string16 override_prompt_text_;
  bool use_override_prompt_text_;

  DISALLOW_COPY_AND_ASSIGN(JavaScriptAppModalDialog);
};

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SHELL_BROWSER_SHELL_JAVASCRIPT_DIALOG_MANAGER_H_
#define CONTENT_SHELL_BROWSER_SHELL_JAVASCRIPT_DIALOG_MANAGER_H_

#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/javascript_dialog_manager.h"

namespace content {

class ShellJavaScriptDialog;

class ShellJavaScriptDialogManager : public JavaScriptDialogManager {
 public:
  ShellJavaScriptDialogManager();
  virtual ~ShellJavaScriptDialogManager();

  
  virtual void RunJavaScriptDialog(
      WebContents* web_contents,
      const GURL& origin_url,
      const std::string& accept_lang,
      JavaScriptMessageType javascript_message_type,
      const base::string16& message_text,
      const base::string16& default_prompt_text,
      const DialogClosedCallback& callback,
      bool* did_suppress_message) OVERRIDE;

  virtual void RunBeforeUnloadDialog(
      WebContents* web_contents,
      const base::string16& message_text,
      bool is_reload,
      const DialogClosedCallback& callback) OVERRIDE;

  virtual void CancelActiveAndPendingDialogs(
      WebContents* web_contents) OVERRIDE;

  virtual void WebContentsDestroyed(WebContents* web_contents) OVERRIDE;

  
  void DialogClosed(ShellJavaScriptDialog* dialog);

  
  void set_dialog_request_callback(const base::Closure& callback) {
    dialog_request_callback_ = callback;
  }

 private:
#if defined(OS_MACOSX) || defined(OS_WIN) || defined(TOOLKIT_GTK)
  
  scoped_ptr<ShellJavaScriptDialog> dialog_;
#else
  
#endif

  base::Closure dialog_request_callback_;

  DISALLOW_COPY_AND_ASSIGN(ShellJavaScriptDialogManager);
};

}  

#endif  

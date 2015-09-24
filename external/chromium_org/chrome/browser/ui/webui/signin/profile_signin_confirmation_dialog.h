// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_SIGNIN_PROFILE_SIGNIN_CONFIRMATION_DIALOG_H_
#define CHROME_BROWSER_UI_WEBUI_SIGNIN_PROFILE_SIGNIN_CONFIRMATION_DIALOG_H_

#include <string>

#include "base/callback.h"
#include "base/strings/string16.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/ui/sync/profile_signin_confirmation_helper.h"
#include "chrome/browser/ui/webui/constrained_web_dialog_ui.h"
#include "ui/base/ui_base_types.h"
#include "ui/web_dialogs/web_dialog_delegate.h"

class Browser;
class Profile;
class WebUIMessageHandler;

namespace content {
class WebContents;
}

class ProfileSigninConfirmationDialog : public ui::WebDialogDelegate {
 public:
  
  static void ShowDialog(
      content::WebContents* web_contents,
      Profile* profile,
      const std::string& username,
      ui::ProfileSigninConfirmationDelegate* delegate);

  
  void Close() const;

 private:
  ProfileSigninConfirmationDialog(
      content::WebContents* web_contents,
      Profile* profile,
      const std::string& username,
      ui::ProfileSigninConfirmationDelegate* delegate);
  virtual ~ProfileSigninConfirmationDialog();

  
  
  
  void Show(bool prompt_for_new_profile);

  
  virtual ui::ModalType GetDialogModalType() const OVERRIDE;
  virtual base::string16 GetDialogTitle() const OVERRIDE;
  virtual GURL GetDialogContentURL() const OVERRIDE;
  virtual void GetWebUIMessageHandlers(
      std::vector<content::WebUIMessageHandler*>* handlers) const OVERRIDE;
  virtual void GetDialogSize(gfx::Size* size) const OVERRIDE;
  virtual std::string GetDialogArgs() const OVERRIDE;
  virtual void OnDialogClosed(const std::string& json_retval) OVERRIDE;
  virtual void OnCloseContents(content::WebContents* source,
                               bool* out_close_dialog) OVERRIDE;
  virtual bool ShouldShowDialogTitle() const OVERRIDE;

  
  content::WebContents* web_contents_;

  
  Profile* profile_;

  
  std::string username_;

  
  ui::ProfileSigninConfirmationDelegate* signin_delegate_;

  
  ConstrainedWebDialogDelegate* dialog_delegate_;

  
  bool prompt_for_new_profile_;

  
  
  mutable bool closed_by_handler_;

  DISALLOW_COPY_AND_ASSIGN(ProfileSigninConfirmationDialog);
};

#endif  

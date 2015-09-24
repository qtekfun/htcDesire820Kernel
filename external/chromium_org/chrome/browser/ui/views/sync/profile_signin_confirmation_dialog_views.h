// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_SYNC_PROFILE_SIGNIN_CONFIRMATION_DIALOG_VIEWS_H_
#define CHROME_BROWSER_UI_VIEWS_SYNC_PROFILE_SIGNIN_CONFIRMATION_DIALOG_VIEWS_H_

#include "base/compiler_specific.h"
#include "chrome/browser/ui/sync/profile_signin_confirmation_helper.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/button/label_button.h"
#include "ui/views/controls/link_listener.h"
#include "ui/views/controls/styled_label_listener.h"
#include "ui/views/window/dialog_delegate.h"

class Browser;
class Profile;

namespace content {
class WebContents;
}

namespace views {
class StyledLabel;
}

class ProfileSigninConfirmationDialogViews : public views::DialogDelegateView,
                                             public views::StyledLabelListener,
                                             public views::ButtonListener {
 public:
  
  static void ShowDialog(Browser* browser,
                         Profile* profile,
                         const std::string& username,
                         ui::ProfileSigninConfirmationDelegate* delegate);

 private:
  ProfileSigninConfirmationDialogViews(
      Browser* browser,
      Profile* profile,
      const std::string& username,
      ui::ProfileSigninConfirmationDelegate* delegate);
  virtual ~ProfileSigninConfirmationDialogViews();

  
  virtual base::string16 GetWindowTitle() const OVERRIDE;
  virtual base::string16 GetDialogButtonLabel(
      ui::DialogButton button) const OVERRIDE;
  virtual int GetDefaultDialogButton() const OVERRIDE;
  virtual views::View* CreateExtraView() OVERRIDE;
  virtual bool Accept() OVERRIDE;
  virtual bool Cancel() OVERRIDE;
  virtual void OnClosed() OVERRIDE;
  virtual ui::ModalType GetModalType() const OVERRIDE;
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;

  
  virtual void StyledLabelLinkClicked(const gfx::Range& range,
                                      int event_flags) OVERRIDE;

  
  virtual void ButtonPressed(views::Button*, const ui::Event& event) OVERRIDE;

  
  
  
  void Show(bool prompt_for_new_profile);

  
  views::StyledLabel* explanation_label_;

  
  Browser* browser_;

  
  Profile* profile_;

  
  std::string username_;

  
  ui::ProfileSigninConfirmationDelegate* delegate_;

  
  bool prompt_for_new_profile_;

  
  views::LabelButton* continue_signin_button_;

  DISALLOW_COPY_AND_ASSIGN(ProfileSigninConfirmationDialogViews);
};

#endif  

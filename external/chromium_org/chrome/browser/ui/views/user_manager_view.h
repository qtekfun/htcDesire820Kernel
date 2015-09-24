// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_USER_MANAGER_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_USER_MANAGER_VIEW_H_

#include "chrome/browser/profiles/profile.h"
#include "ui/views/window/dialog_delegate.h"

namespace views {
class WebView;
}

class UserManagerView : public views::DialogDelegateView {
 public:
  
  static void Show(const base::FilePath& profile_path_to_focus);

  
  static void Hide();

  
  static bool IsShowing();

 private:
  explicit UserManagerView(Profile* profile);
  virtual ~UserManagerView();

  
  
  
  static void OnGuestProfileCreated(const base::FilePath& profile_path_to_focus,
                                    Profile* guest_profile,
                                    Profile::CreateStatus status);

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;

  
  virtual bool CanResize() const OVERRIDE;
  virtual bool CanMaximize() const OVERRIDE;
  virtual base::string16 GetWindowTitle() const OVERRIDE;
  virtual int GetDialogButtons() const OVERRIDE;
  virtual void WindowClosing() OVERRIDE;
  virtual bool UseNewStyleForThisDialog() const OVERRIDE;

  views::WebView* web_view_;

  
  
  static UserManagerView* instance_;

  DISALLOW_COPY_AND_ASSIGN(UserManagerView);
};

#endif  

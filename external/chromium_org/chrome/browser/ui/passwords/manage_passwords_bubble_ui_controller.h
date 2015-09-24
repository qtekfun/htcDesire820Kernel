// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_PASSWORDS_MANAGE_PASSWORDS_BUBBLE_UI_CONTROLLER_H_
#define CHROME_BROWSER_UI_PASSWORDS_MANAGE_PASSWORDS_BUBBLE_UI_CONTROLLER_H_

#include "chrome/browser/password_manager/password_form_manager.h"
#include "content/public/browser/navigation_details.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

namespace content {
class WebContents;
}

class ManagePasswordsBubbleUIController
    : public content::WebContentsObserver,
      public content::WebContentsUserData<ManagePasswordsBubbleUIController> {
 public:
  virtual ~ManagePasswordsBubbleUIController();

  
  
  
  
  void OnPasswordSubmitted(PasswordFormManager* form_manager);

  
  
  
  
  void OnPasswordAutofilled(const autofill::PasswordFormMap& password_form_map);

  
  
  
  
  
  void RemoveFromBestMatches(autofill::PasswordForm password_form);

  void SavePassword();

  
  
  void OnBubbleShown();

  bool manage_passwords_icon_to_be_shown() const {
    return manage_passwords_icon_to_be_shown_;
  }

  bool password_to_be_saved() const {
    return password_to_be_saved_;
  }

  bool manage_passwords_bubble_needs_showing() const {
    return manage_passwords_bubble_needs_showing_;
  }

  void unset_manage_passwords_bubble_needs_showing() {
    manage_passwords_bubble_needs_showing_ = false;
  }

  void unset_password_to_be_saved() {
    password_to_be_saved_ = false;
  }

  const autofill::PasswordForm pending_credentials() const {
    return form_manager_->pending_credentials();
  }

  const autofill::PasswordFormMap best_matches() const {
    return password_form_map_;
  }

  bool password_submitted() const {
    return password_submitted_;
  }

  void set_password_submitted(bool password_submitted) {
    password_submitted_ = password_submitted;
  }

 private:
  friend class content::WebContentsUserData<ManagePasswordsBubbleUIController>;

  explicit ManagePasswordsBubbleUIController(
      content::WebContents* web_contents);

  
  
  
  void UpdateBubbleAndIconVisibility();

  
  virtual void DidNavigateMainFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) OVERRIDE;

  
  
  
  
  scoped_ptr<PasswordFormManager> form_manager_;

  
  
  autofill::PasswordFormMap password_form_map_;

  bool manage_passwords_icon_to_be_shown_;
  bool password_to_be_saved_;
  bool manage_passwords_bubble_needs_showing_;
  
  
  bool password_submitted_;

  DISALLOW_COPY_AND_ASSIGN(ManagePasswordsBubbleUIController);
};

#endif  

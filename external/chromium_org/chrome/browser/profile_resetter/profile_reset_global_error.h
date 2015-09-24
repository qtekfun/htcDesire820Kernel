// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILE_RESETTER_PROFILE_RESET_GLOBAL_ERROR_H_
#define CHROME_BROWSER_PROFILE_RESETTER_PROFILE_RESET_GLOBAL_ERROR_H_

#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"
#include "base/timer/elapsed_timer.h"
#include "chrome/browser/ui/global_error/global_error.h"

class AutomaticProfileResetter;
class GlobalErrorBubbleViewBase;
class Profile;

class ProfileResetGlobalError
    : public GlobalError,
      public base::SupportsWeakPtr<ProfileResetGlobalError> {
 public:
  explicit ProfileResetGlobalError(Profile* profile);
  virtual ~ProfileResetGlobalError();

  
  static bool IsSupportedOnPlatform();

  
  void OnBubbleViewDidClose();

  
  
  
  void OnBubbleViewResetButtonPressed(bool send_feedback);

  
  void OnBubbleViewNoThanksButtonPressed();

  
  virtual bool HasMenuItem() OVERRIDE;
  virtual int MenuItemCommandID() OVERRIDE;
  virtual base::string16 MenuItemLabel() OVERRIDE;
  virtual void ExecuteMenuItem(Browser* browser) OVERRIDE;
  virtual bool HasBubbleView() OVERRIDE;
  virtual bool HasShownBubbleView() OVERRIDE;
  virtual void ShowBubbleView(Browser* browser) OVERRIDE;
  virtual GlobalErrorBubbleViewBase* GetBubbleView() OVERRIDE;

 private:
  Profile* profile_;

  
  
  
  
  base::WeakPtr<AutomaticProfileResetter> automatic_profile_resetter_;

  
  base::ElapsedTimer timer_;

  
  bool has_shown_bubble_view_;

  
  GlobalErrorBubbleViewBase* bubble_view_;

  DISALLOW_COPY_AND_ASSIGN(ProfileResetGlobalError);
};

#endif  

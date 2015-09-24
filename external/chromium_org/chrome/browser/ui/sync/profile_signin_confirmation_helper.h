// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SYNC_PROFILE_SIGNIN_CONFIRMATION_HELPER_H_
#define CHROME_BROWSER_UI_SYNC_PROFILE_SIGNIN_CONFIRMATION_HELPER_H_

#include "base/callback.h"
#include "third_party/skia/include/core/SkColor.h"

class Profile;

namespace ui {

const SkAlpha kSigninConfirmationPromptBarBackgroundAlpha = 0x0A;
const SkAlpha kSigninConfirmationPromptBarBorderAlpha = 0x1F;

SkColor GetSigninConfirmationPromptBarColor(SkAlpha alpha);

bool HasBeenShutdown(Profile* profile);

bool HasSyncedExtensions(Profile* profile);

void CheckShouldPromptForNewProfile(
    Profile* profile,
    const base::Callback<void(bool)>& cb);

class ProfileSigninConfirmationDelegate {
 public:
  virtual void OnCancelSignin() = 0;
  virtual void OnContinueSignin() = 0;
  virtual void OnSigninWithNewProfile() = 0;
};

}  

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_AVATAR_MENU_ACTIONS_DESKTOP_H_
#define CHROME_BROWSER_PROFILES_AVATAR_MENU_ACTIONS_DESKTOP_H_

#include <string>

#include "chrome/browser/profiles/avatar_menu_actions.h"
#include "chrome/browser/profiles/profile_metrics.h"

class Profile;

class AvatarMenuActionsDesktop : public AvatarMenuActions {
 public:
  AvatarMenuActionsDesktop();
  virtual ~AvatarMenuActionsDesktop();

  
  virtual void AddNewProfile(ProfileMetrics::ProfileAdd type) OVERRIDE;
  virtual void EditProfile(Profile* profile, size_t index) OVERRIDE;
  virtual bool ShouldShowAddNewProfileLink() const OVERRIDE;
  virtual bool ShouldShowEditProfileLink() const OVERRIDE;
  virtual void ActiveBrowserChanged(Browser* browser) OVERRIDE;

 private:
  
  Browser* browser_;

  
  std::string logout_override_;

  DISALLOW_COPY_AND_ASSIGN(AvatarMenuActionsDesktop);
};

#endif  

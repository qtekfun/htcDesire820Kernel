// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_PROFILES_AVATAR_MENU_ACTIONS_CHROMEOS_H_
#define CHROME_BROWSER_CHROMEOS_PROFILES_AVATAR_MENU_ACTIONS_CHROMEOS_H_

#include "chrome/browser/profiles/avatar_menu_actions.h"

#include <string>

class Browser;

namespace chromeos {

class AvatarMenuActionsChromeOS : public AvatarMenuActions {
 public:
  AvatarMenuActionsChromeOS();
  virtual ~AvatarMenuActionsChromeOS();

  
  virtual void AddNewProfile(ProfileMetrics::ProfileAdd type) OVERRIDE;
  virtual void EditProfile(Profile* profile, size_t index) OVERRIDE;
  virtual bool ShouldShowAddNewProfileLink() const OVERRIDE;
  virtual bool ShouldShowEditProfileLink() const OVERRIDE;
  virtual void ActiveBrowserChanged(Browser* browser) OVERRIDE;

 private:
  
  Browser* browser_;

  DISALLOW_COPY_AND_ASSIGN(AvatarMenuActionsChromeOS);
};

}  

#endif  

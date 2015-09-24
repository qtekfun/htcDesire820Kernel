// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_AVATAR_MENU_ACTIONS_H_
#define CHROME_BROWSER_PROFILES_AVATAR_MENU_ACTIONS_H_

#include <string>

#include "chrome/browser/profiles/avatar_menu.h"
#include "chrome/browser/profiles/profile_metrics.h"

class Profile;

class AvatarMenuActions {
 public:
  virtual ~AvatarMenuActions() {}

  static AvatarMenuActions* Create();

  
  virtual void AddNewProfile(ProfileMetrics::ProfileAdd type) = 0;

  
  virtual void EditProfile(Profile* profile, size_t index) = 0;

  
  virtual bool ShouldShowAddNewProfileLink() const = 0;

  
  virtual bool ShouldShowEditProfileLink() const = 0;

  
  
  virtual void ActiveBrowserChanged(Browser* browser) = 0;
};

#endif  

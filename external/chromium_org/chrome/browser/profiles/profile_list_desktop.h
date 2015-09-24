// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_PROFILE_LIST_DESKTOP_H_
#define CHROME_BROWSER_PROFILES_PROFILE_LIST_DESKTOP_H_

#include "chrome/browser/profiles/profile_list.h"

#include <vector>

class Browser;
class ProfileInfoInterface;

class ProfileListDesktop : public ProfileList {
 public:
  explicit ProfileListDesktop(ProfileInfoInterface* profile_cache);
  virtual ~ProfileListDesktop();

  
  virtual size_t GetNumberOfItems() const OVERRIDE;
  virtual const AvatarMenu::Item& GetItemAt(size_t index) const OVERRIDE;
  virtual void RebuildMenu() OVERRIDE;
  virtual size_t MenuIndexFromProfileIndex(size_t index) OVERRIDE;
  virtual void ActiveProfilePathChanged(base::FilePath& path) OVERRIDE;

 private:
  void ClearMenu();

  
  ProfileInfoInterface* profile_info_;

  
  base::FilePath active_profile_path_;

  
  std::vector<AvatarMenu::Item*> items_;

  DISALLOW_COPY_AND_ASSIGN(ProfileListDesktop);
};

#endif  

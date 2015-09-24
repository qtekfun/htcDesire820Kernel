// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_PROFILES_PROFILE_LIST_CHROMEOS_H_
#define CHROME_BROWSER_CHROMEOS_PROFILES_PROFILE_LIST_CHROMEOS_H_

#include "chrome/browser/profiles/profile_list.h"

#include <vector>

class ProfileInfoInterface;

namespace chromeos {

class ProfileListChromeOS : public ProfileList {
 public:
  explicit ProfileListChromeOS(ProfileInfoInterface* profile_cache);
  virtual ~ProfileListChromeOS();

  
  virtual size_t GetNumberOfItems() const OVERRIDE;
  virtual const AvatarMenu::Item& GetItemAt(size_t index) const OVERRIDE;
  virtual void RebuildMenu() OVERRIDE;
  virtual size_t MenuIndexFromProfileIndex(size_t index) OVERRIDE;
  virtual void ActiveProfilePathChanged(base::FilePath& path) OVERRIDE;

 private:
  void ClearMenu();
  void SortMenu();

  
  ProfileInfoInterface* profile_info_;

  
  base::FilePath active_profile_path_;

  
  std::vector<AvatarMenu::Item*> items_;

  DISALLOW_COPY_AND_ASSIGN(ProfileListChromeOS);
};

}  

#endif  

// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_PROFILE_LIST_H_
#define CHROME_BROWSER_PROFILES_PROFILE_LIST_H_

#include "chrome/browser/profiles/avatar_menu.h"

class ProfileInfoInterface;

class ProfileList {
 public:
  virtual ~ProfileList() {}

  static ProfileList* Create(ProfileInfoInterface* profile_cache);

  
  virtual size_t GetNumberOfItems() const = 0;

  
  virtual const AvatarMenu::Item& GetItemAt(size_t index) const = 0;

  
  virtual void RebuildMenu() = 0;

  
  virtual size_t MenuIndexFromProfileIndex(size_t index) = 0;

  
  virtual void ActiveProfilePathChanged(base::FilePath& path) = 0;
};

#endif  

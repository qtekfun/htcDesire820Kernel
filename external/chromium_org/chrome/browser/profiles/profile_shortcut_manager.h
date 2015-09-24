// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_PROFILE_SHORTCUT_MANAGER_H_
#define CHROME_BROWSER_PROFILES_PROFILE_SHORTCUT_MANAGER_H_

#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/strings/string16.h"
#include "chrome/browser/profiles/profile_info_cache.h"

class CommandLine;
class ProfileManager;

class ProfileShortcutManager {
 public:
  virtual ~ProfileShortcutManager();

  
  virtual void CreateOrUpdateProfileIcon(
      const base::FilePath& profile_path) = 0;

  
  
  
  virtual void CreateProfileShortcut(const base::FilePath& profile_path) = 0;

  
  
  virtual void RemoveProfileShortcuts(const base::FilePath& profile_path) = 0;

  
  
  
  virtual void HasProfileShortcuts(
      const base::FilePath& profile_path,
      const base::Callback<void(bool)>& callback) = 0;

  
  
  virtual void GetShortcutProperties(const base::FilePath& profile_path,
                                     CommandLine* command_line,
                                     base::string16* name,
                                     base::FilePath* icon_path) = 0;

  static bool IsFeatureEnabled();
  static ProfileShortcutManager* Create(ProfileManager* manager);

 protected:
  ProfileShortcutManager();

 private:
  DISALLOW_COPY_AND_ASSIGN(ProfileShortcutManager);
};

#endif  

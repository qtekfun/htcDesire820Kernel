// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_PROFILE_SHORTCUT_MANAGER_WIN_H_
#define CHROME_BROWSER_PROFILES_PROFILE_SHORTCUT_MANAGER_WIN_H_

#include "base/callback.h"
#include "chrome/browser/profiles/profile_shortcut_manager.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class BrowserDistribution;

namespace profiles {
namespace internal {

base::FilePath GetProfileIconPath(const base::FilePath& profile_path);

base::string16 GetShortcutFilenameForProfile(const base::string16& profile_name,
                                             BrowserDistribution* distribution);

base::string16 CreateProfileShortcutFlags(const base::FilePath& profile_path);

}  
}  

class ProfileShortcutManagerWin : public ProfileShortcutManager,
                                  public ProfileInfoCacheObserver,
                                  public content::NotificationObserver {
 public:
  
  
  
  enum CreateOrUpdateMode {
    UPDATE_EXISTING_ONLY,
    CREATE_WHEN_NONE_FOUND,
    CREATE_OR_UPDATE_ICON_ONLY,
  };
  
  enum NonProfileShortcutAction {
    IGNORE_NON_PROFILE_SHORTCUTS,
    UPDATE_NON_PROFILE_SHORTCUTS,
  };

  explicit ProfileShortcutManagerWin(ProfileManager* manager);
  virtual ~ProfileShortcutManagerWin();

  
  virtual void CreateOrUpdateProfileIcon(
      const base::FilePath& profile_path) OVERRIDE;
  virtual void CreateProfileShortcut(
      const base::FilePath& profile_path) OVERRIDE;
  virtual void RemoveProfileShortcuts(
      const base::FilePath& profile_path) OVERRIDE;
  virtual void HasProfileShortcuts(
      const base::FilePath& profile_path,
      const base::Callback<void(bool)>& callback) OVERRIDE;
  virtual void GetShortcutProperties(const base::FilePath& profile_path,
                                     CommandLine* command_line,
                                     base::string16* name,
                                     base::FilePath* icon_path) OVERRIDE;

  
  virtual void OnProfileAdded(const base::FilePath& profile_path) OVERRIDE;
  virtual void OnProfileWasRemoved(
      const base::FilePath& profile_path,
      const base::string16& profile_name) OVERRIDE;
  virtual void OnProfileNameChanged(
      const base::FilePath& profile_path,
      const base::string16& old_profile_name) OVERRIDE;
  virtual void OnProfileAvatarChanged(
      const base::FilePath& profile_path) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  
  
  base::FilePath GetOtherProfilePath(const base::FilePath& profile_path);

  
  
  
  void CreateOrUpdateShortcutsForProfileAtPath(
      const base::FilePath& profile_path,
      CreateOrUpdateMode create_mode,
      NonProfileShortcutAction action);

  ProfileManager* profile_manager_;

  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(ProfileShortcutManagerWin);
};

#endif  

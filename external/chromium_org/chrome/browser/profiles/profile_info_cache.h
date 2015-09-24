// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_PROFILE_INFO_CACHE_H_
#define CHROME_BROWSER_PROFILES_PROFILE_INFO_CACHE_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/strings/string16.h"
#include "chrome/browser/profiles/profile_info_cache_observer.h"
#include "chrome/browser/profiles/profile_info_interface.h"

namespace gfx {
class Image;
}

namespace base {
class DictionaryValue;
}

class PrefService;
class PrefRegistrySimple;

class ProfileInfoCache : public ProfileInfoInterface,
                         public base::SupportsWeakPtr<ProfileInfoCache> {
 public:
  ProfileInfoCache(PrefService* prefs, const base::FilePath& user_data_dir);
  virtual ~ProfileInfoCache();

  
  
  void AddProfileToCache(const base::FilePath& profile_path,
                         const base::string16& name,
                         const base::string16& username,
                         size_t icon_index,
                         const std::string& managed_user_id);
  void DeleteProfileFromCache(const base::FilePath& profile_path);

  
  virtual size_t GetNumberOfProfiles() const OVERRIDE;
  
  
  virtual size_t GetIndexOfProfileWithPath(
      const base::FilePath& profile_path) const OVERRIDE;
  virtual base::string16 GetNameOfProfileAtIndex(size_t index) const OVERRIDE;
  virtual base::string16 GetShortcutNameOfProfileAtIndex(size_t index)
      const OVERRIDE;
  virtual base::FilePath GetPathOfProfileAtIndex(size_t index) const OVERRIDE;
  virtual base::string16 GetUserNameOfProfileAtIndex(
      size_t index) const OVERRIDE;
  virtual const gfx::Image& GetAvatarIconOfProfileAtIndex(
      size_t index) const OVERRIDE;
  virtual std::string GetLocalAuthCredentialsOfProfileAtIndex(
      size_t index) const OVERRIDE;
  
  
  
  virtual bool GetBackgroundStatusOfProfileAtIndex(
      size_t index) const OVERRIDE;
  virtual base::string16 GetGAIANameOfProfileAtIndex(
      size_t index) const OVERRIDE;
  virtual base::string16 GetGAIAGivenNameOfProfileAtIndex(
      size_t index) const OVERRIDE;
  virtual bool IsUsingGAIANameOfProfileAtIndex(size_t index) const OVERRIDE;
  
  
  
  virtual const gfx::Image* GetGAIAPictureOfProfileAtIndex(
      size_t index) const OVERRIDE;
  virtual bool IsUsingGAIAPictureOfProfileAtIndex(
      size_t index) const OVERRIDE;
  virtual bool ProfileIsManagedAtIndex(size_t index) const OVERRIDE;
  virtual bool ProfileIsSigninRequiredAtIndex(size_t index) const OVERRIDE;
  virtual std::string GetManagedUserIdOfProfileAtIndex(size_t index) const
      OVERRIDE;
  virtual bool ProfileIsEphemeralAtIndex(size_t index) const OVERRIDE;

  size_t GetAvatarIconIndexOfProfileAtIndex(size_t index) const;

  void SetNameOfProfileAtIndex(size_t index, const base::string16& name);
  void SetShortcutNameOfProfileAtIndex(size_t index,
                                       const base::string16& name);
  void SetUserNameOfProfileAtIndex(size_t index,
                                   const base::string16& user_name);
  void SetAvatarIconOfProfileAtIndex(size_t index, size_t icon_index);
  void SetManagedUserIdOfProfileAtIndex(size_t index, const std::string& id);
  void SetLocalAuthCredentialsOfProfileAtIndex(size_t index,
                                               const std::string& auth);
  void SetBackgroundStatusOfProfileAtIndex(size_t index,
                                           bool running_background_apps);
  void SetGAIANameOfProfileAtIndex(size_t index, const base::string16& name);
  void SetGAIAGivenNameOfProfileAtIndex(size_t index,
                                        const base::string16& name);
  void SetIsUsingGAIANameOfProfileAtIndex(size_t index, bool value);
  void SetGAIAPictureOfProfileAtIndex(size_t index, const gfx::Image* image);
  void SetIsUsingGAIAPictureOfProfileAtIndex(size_t index, bool value);
  void SetProfileSigninRequiredAtIndex(size_t index, bool value);
  void SetProfileIsEphemeralAtIndex(size_t index, bool value);

  
  base::string16 ChooseNameForNewProfile(size_t icon_index) const;

  
  
  
  
  bool GetHasMigratedToGAIAInfoOfProfileAtIndex(size_t index) const;

  
  
  void SetHasMigratedToGAIAInfoOfProfileAtIndex(size_t index, bool value);

  
  
  
  size_t ChooseAvatarIconIndexForNewProfile() const;

  const base::FilePath& GetUserDataDir() const;

  
  static size_t GetDefaultAvatarIconCount();
  
  static int GetDefaultAvatarIconResourceIDAtIndex(size_t index);
  
  static std::string GetDefaultAvatarIconUrl(size_t index);
  
  static bool IsDefaultAvatarIconIndex(size_t index);
  
  
  static bool IsDefaultAvatarIconUrl(const std::string& icon_url,
                                     size_t *icon_index);

  
  
  
  
  static std::vector<base::string16> GetProfileNames();

  
  static void RegisterPrefs(PrefRegistrySimple* registry);

  void AddObserver(ProfileInfoCacheObserver* obs);
  void RemoveObserver(ProfileInfoCacheObserver* obs);

 private:
  const base::DictionaryValue* GetInfoForProfileAtIndex(size_t index) const;
  
  
  
  void SetInfoForProfileAtIndex(size_t index, base::DictionaryValue* info);
  std::string CacheKeyFromProfilePath(const base::FilePath& profile_path) const;
  std::vector<std::string>::iterator FindPositionForProfile(
      const std::string& search_key,
      const base::string16& search_name);

  
  bool IconIndexIsUnique(size_t icon_index) const;

  
  
  bool ChooseAvatarIconIndexForNewProfile(bool allow_generic_icon,
                                          bool must_be_unique,
                                          size_t* out_icon_index) const;

  
  
  void UpdateSortForProfileIndex(size_t index);

  void OnGAIAPictureLoaded(const base::FilePath& path,
                           gfx::Image** image) const;
  void OnGAIAPictureSaved(const base::FilePath& path, bool* success) const;

  PrefService* prefs_;
  std::vector<std::string> sorted_keys_;
  base::FilePath user_data_dir_;

  ObserverList<ProfileInfoCacheObserver> observer_list_;

  
  
  mutable std::map<std::string, gfx::Image*> gaia_pictures_;
  
  
  mutable std::map<std::string, bool> gaia_pictures_loading_;

  DISALLOW_COPY_AND_ASSIGN(ProfileInfoCache);
};

#endif  

// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_USER_IMAGE_MANAGER_IMPL_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_USER_IMAGE_MANAGER_IMPL_H_

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "base/values.h"
#include "chrome/browser/chromeos/login/user.h"
#include "chrome/browser/chromeos/login/user_image_loader.h"
#include "chrome/browser/chromeos/login/user_image_manager.h"
#include "chrome/browser/chromeos/policy/cloud_external_data_policy_observer.h"
#include "chrome/browser/profiles/profile_downloader_delegate.h"
#include "ui/gfx/image/image_skia.h"

class ProfileDownloader;
class UserImage;

namespace base {
class FilePath;
class SequencedTaskRunner;
}

namespace chromeos {

class CrosSettings;
class UserImageSyncObserver;
class UserManager;

class UserImageManagerImpl
    : public UserImageManager,
      public ProfileDownloaderDelegate,
      public policy::CloudExternalDataPolicyObserver::Delegate {
 public:
  UserImageManagerImpl(CrosSettings* cros_settings, UserManager* user_manager);

  
  virtual ~UserImageManagerImpl();
  virtual void LoadUserImages(const UserList& users) OVERRIDE;
  virtual void UserLoggedIn(const std::string& user_id,
                            bool user_is_new,
                            bool user_is_local) OVERRIDE;
  virtual void SaveUserDefaultImageIndex(const std::string& user_id,
                                         int default_image_index) OVERRIDE;
  virtual void SaveUserImage(const std::string& user_id,
                             const UserImage& user_image) OVERRIDE;
  virtual void SaveUserImageFromFile(const std::string& user_id,
                                     const base::FilePath& path) OVERRIDE;
  virtual void SaveUserImageFromProfileImage(
      const std::string& user_id) OVERRIDE;
  virtual void DeleteUserImage(const std::string& user_id) OVERRIDE;
  virtual void DownloadProfileImage(const std::string& reason) OVERRIDE;
  virtual const gfx::ImageSkia& DownloadedProfileImage() const OVERRIDE;
  virtual UserImageSyncObserver* GetSyncObserver() const OVERRIDE;
  virtual void Shutdown() OVERRIDE;

  
  virtual void OnExternalDataSet(const std::string& policy,
                                 const std::string& user_id) OVERRIDE;
  virtual void OnExternalDataCleared(const std::string& policy,
                                     const std::string& user_id) OVERRIDE;
  virtual void OnExternalDataFetched(const std::string& policy,
                                     const std::string& user_id,
                                     scoped_ptr<std::string> data) OVERRIDE;

  static void IgnoreProfileDataDownloadDelayForTesting();
  void StopPolicyObserverForTesting();

 private:
  friend class UserImageManagerTest;

  
  
  
  
  
  
  
  
  
  
  
  class Job;

  
  virtual bool NeedsProfilePicture() const OVERRIDE;
  virtual int GetDesiredImageSideLength() const OVERRIDE;
  virtual Profile* GetBrowserProfile() OVERRIDE;
  virtual std::string GetCachedPictureURL() const OVERRIDE;
  virtual void OnProfileDownloadSuccess(ProfileDownloader* downloader) OVERRIDE;
  virtual void OnProfileDownloadFailure(
      ProfileDownloader* downloader,
      ProfileDownloaderDelegate::FailureReason reason) OVERRIDE;

  
  
  bool IsUserImageManaged(const std::string& user_id) const;

  
  
  void SetInitialUserImage(const std::string& user_id);

  
  
  
  void TryToInitDownloadedProfileImage();

  
  
  
  
  
  bool NeedProfileImage() const;

  
  
  
  
  void DownloadProfileData(const std::string& reason);

  
  
  void DeleteUserImageAndLocalStateEntry(const std::string& user_id,
                                         const char* prefs_dict_root);

  
  
  
  void OnJobChangedUserImage(const User* user);

  
  
  
  void OnJobDone(const std::string& user_id);

  
  void UpdateLocalStateAfterMigration(const std::string& user_id);

  
  
  void TryToCreateImageSyncObserver();

  
  UserManager* user_manager_;

  
  scoped_refptr<UserImageLoader> image_loader_;

  
  scoped_refptr<UserImageLoader> unsafe_image_loader_;

  
  
  
  bool downloading_profile_image_;

  
  
  
  std::string profile_image_download_reason_;

  
  
  base::TimeTicks profile_image_load_start_time_;

  
  
  scoped_ptr<ProfileDownloader> profile_downloader_;

  
  
  gfx::ImageSkia downloaded_profile_image_;

  
  
  std::string downloaded_profile_image_data_url_;

  
  
  GURL profile_image_url_;

  
  
  
  bool profile_image_requested_;

  
  
  base::OneShotTimer<UserImageManagerImpl> profile_download_one_shot_timer_;

  
  
  base::RepeatingTimer<UserImageManagerImpl> profile_download_periodic_timer_;

  
  
  std::set<std::string> users_to_migrate_;

  
  scoped_ptr<UserImageSyncObserver> user_image_sync_observer_;

  
  scoped_ptr<policy::CloudExternalDataPolicyObserver> policy_observer_;

  
  
  scoped_refptr<base::SequencedTaskRunner> background_task_runner_;

  
  std::map<std::string, linked_ptr<Job> > jobs_;

  
  std::set<std::string> users_with_managed_images_;

  base::WeakPtrFactory<UserImageManagerImpl> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(UserImageManagerImpl);
};

}  

#endif  

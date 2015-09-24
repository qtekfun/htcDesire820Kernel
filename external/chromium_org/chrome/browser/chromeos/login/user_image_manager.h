// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_USER_IMAGE_MANAGER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_USER_IMAGE_MANAGER_H_

#include <string>

#include "chrome/browser/chromeos/login/user.h"

class PrefRegistrySimple;

namespace base {
class FilePath;
}

namespace gfx {
class ImageSkia;
}

namespace chromeos {

class UserImage;
class UserImageSyncObserver;

class UserImageManager {
 public:
  
  static void RegisterPrefs(PrefRegistrySimple* registry);

  virtual ~UserImageManager();

  
  virtual void LoadUserImages(const UserList& users) = 0;

  
  virtual void UserLoggedIn(const std::string& user_id,
                            bool user_is_new,
                            bool user_is_local) = 0;

  
  
  virtual void SaveUserDefaultImageIndex(const std::string& user_id,
                                         int image_index) = 0;

  
  
  virtual void SaveUserImage(const std::string& user_id,
                             const UserImage& user_image) = 0;

  
  
  virtual void SaveUserImageFromFile(const std::string& user_id,
                                     const base::FilePath& path) = 0;

  
  
  
  
  
  virtual void SaveUserImageFromProfileImage(const std::string& user_id) = 0;

  
  virtual void DeleteUserImage(const std::string& user_id) = 0;

  
  
  
  
  
  virtual void DownloadProfileImage(const std::string& reason) = 0;

  
  
  virtual const gfx::ImageSkia& DownloadedProfileImage() const = 0;

  
  
  virtual UserImageSyncObserver* GetSyncObserver() const = 0;

  
  
  virtual void Shutdown() = 0;
};

}  

#endif  

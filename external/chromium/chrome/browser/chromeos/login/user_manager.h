// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_USER_MANAGER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_USER_MANAGER_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "chrome/browser/chromeos/login/user_image_loader.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "third_party/skia/include/core/SkBitmap.h"

class FilePath;
class PrefService;

namespace base {
template<typename> struct DefaultLazyInstanceTraits;
}

namespace chromeos {
class RemoveUserDelegate;

class UserManager : public UserImageLoader::Delegate,
                    public NotificationObserver {
 public:
  
  class User {
   public:
    User();
    ~User();

    
    void set_email(const std::string& email) { email_ = email; }
    const std::string& email() const { return email_; }

    
    std::string GetDisplayName() const;

    
    
    std::string GetNameTooltip() const;

    
    void set_image(const SkBitmap& image) { image_ = image; }
    const SkBitmap& image() const { return image_; }

   private:
    std::string email_;
    SkBitmap image_;
  };

  
  
  static UserManager* Get();

  
  static void RegisterPrefs(PrefService* local_state);

  
  
  virtual std::vector<User> GetUsers() const;

  
  virtual void OffTheRecordUserLoggedIn();

  
  
  virtual void UserLoggedIn(const std::string& email);

  
  
  
  virtual void RemoveUser(const std::string& email,
                          RemoveUserDelegate* delegate);

  
  
  virtual void RemoveUserFromList(const std::string& email);

  
  virtual bool IsKnownUser(const std::string& email);

  
  virtual const User& logged_in_user() const;

  
  
  void SetLoggedInUserImage(const SkBitmap& image);

  
  void LoadLoggedInUserImage(const FilePath& path);

  
  void SaveUserImage(const std::string& username,
                     const SkBitmap& image);

  
  void SaveUserImagePath(const std::string& username,
                         const std::string& image_path);

  
  
  int GetUserDefaultImageIndex(const std::string& username);

  
  virtual void OnImageLoaded(const std::string& username,
                             const SkBitmap& image,
                             bool save_image);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual bool current_user_is_owner() const;
  virtual void set_current_user_is_owner(bool current_user_is_owner);

  
  bool current_user_is_new() const {
    return current_user_is_new_;
  }

  bool user_is_logged_in() const { return user_is_logged_in_; }

  
  bool IsLoggedInAsGuest() const;

 protected:
  UserManager();
  virtual ~UserManager();

  
  FilePath GetImagePathForUser(const std::string& username);

 private:
  
  void NotifyOnLogin();

  
  
  void SetDefaultUserImage(const std::string& username);

  
  scoped_refptr<UserImageLoader> image_loader_;

  
  typedef base::hash_map<std::string, SkBitmap> UserImages;
  mutable UserImages user_images_;

  
  User logged_in_user_;

  
  
  bool current_user_is_owner_;
  mutable base::Lock current_user_is_owner_lock_;

  
  
  bool current_user_is_new_;

  
  bool user_is_logged_in_;

  NotificationRegistrar registrar_;

  friend struct base::DefaultLazyInstanceTraits<UserManager>;

  DISALLOW_COPY_AND_ASSIGN(UserManager);
};

typedef std::vector<UserManager::User> UserVector;

}  

#endif  

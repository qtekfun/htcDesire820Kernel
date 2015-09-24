// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_USER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_USER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "chrome/browser/chromeos/login/user_image.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/image/image_skia.h"

namespace chromeos {

extern const int kDefaultImagesCount;

struct UserContext {
  UserContext();
  UserContext(const std::string& username,
              const std::string& password,
              const std::string& auth_code);
  UserContext(const std::string& username,
              const std::string& password,
              const std::string& auth_code,
              const std::string& username_hash);
  UserContext(const std::string& username,
              const std::string& password,
              const std::string& auth_code,
              const std::string& username_hash,
              bool using_oauth);
  virtual ~UserContext();
  bool operator==(const UserContext& context) const;
  std::string username;
  std::string password;
  std::string auth_code;
  std::string username_hash;
  bool using_oauth;
};

class User {
 public:
  
  typedef enum {
    
    USER_TYPE_REGULAR = 0,
    
    USER_TYPE_GUEST = 1,
    
    
    USER_TYPE_RETAIL_MODE = 2,
    
    
    USER_TYPE_PUBLIC_ACCOUNT = 3,
    
    USER_TYPE_LOCALLY_MANAGED = 4,
    
    USER_TYPE_KIOSK_APP = 5,
    
    NUM_USER_TYPES = 6
  } UserType;

  
  
  
  typedef enum {
     OAUTH_TOKEN_STATUS_UNKNOWN  = 0,
     OAUTH2_TOKEN_STATUS_INVALID = 3,
     OAUTH2_TOKEN_STATUS_VALID   = 4,
  } OAuthTokenStatus;

  
  
  static const int kExternalImageIndex = -1;
  
  static const int kProfileImageIndex = -2;
  static const int kInvalidImageIndex = -3;

  enum WallpaperType {
    DAILY = 0,
    CUSTOMIZED = 1,
    DEFAULT = 2,
    UNKNOWN = 3,
    ONLINE = 4,
    WALLPAPER_TYPE_COUNT = 5
  };

  
  virtual UserType GetType() const = 0;

  
  const std::string& email() const { return email_; }

  
  base::string16 GetDisplayName() const;

  
  const base::string16& given_name() const { return given_name_; }

  
  
  std::string GetAccountName(bool use_display_email) const;

  
  const gfx::ImageSkia& image() const { return user_image_.image(); }

  
  bool HasDefaultImage() const;

  
  virtual bool CanSyncImage() const;

  int image_index() const { return image_index_; }
  bool has_raw_image() const { return user_image_.has_raw_image(); }
  
  const UserImage::RawImage& raw_image() const {
    return user_image_.raw_image();
  }
  bool has_animated_image() const { return user_image_.has_animated_image(); }
  
  const UserImage::RawImage& animated_image() const {
    return user_image_.animated_image();
  }

  
  
  bool image_is_safe_format() const { return user_image_.is_safe_format(); }

  
  
  GURL image_url() const { return user_image_.url(); }

  
  bool image_is_stub() const { return image_is_stub_; }

  
  bool image_is_loading() const { return image_is_loading_; }

  
  OAuthTokenStatus oauth_token_status() const { return oauth_token_status_; }

  
  base::string16 display_name() const { return display_name_; }

  
  virtual std::string display_email() const;

  
  
  virtual bool can_lock() const;

  virtual std::string username_hash() const;

  
  virtual bool is_logged_in() const;

  
  virtual bool is_active() const;

 protected:
  friend class SupervisedUserManagerImpl;
  friend class UserManagerImpl;
  friend class UserImageManagerImpl;
  
  friend class MockUserManager;
  friend class FakeUserManager;

  
  static User* CreateRegularUser(const std::string& email);
  static User* CreateGuestUser();
  static User* CreateKioskAppUser(const std::string& kiosk_app_username);
  static User* CreateLocallyManagedUser(const std::string& username);
  static User* CreateRetailModeUser();
  static User* CreatePublicAccountUser(const std::string& email);

  explicit User(const std::string& email);
  virtual ~User();

  bool is_profile_created() const {
    return profile_is_created_;
  }

  const std::string* GetAccountLocale() const {
    return account_locale_.get();
  }

  
  void SetAccountLocale(const std::string& resolved_account_locale);

  void SetImage(const UserImage& user_image, int image_index);

  void SetImageURL(const GURL& image_url);

  
  
  
  void SetStubImage(int image_index, bool is_loading);

  void set_oauth_token_status(OAuthTokenStatus status) {
    oauth_token_status_ = status;
  }

  void set_display_name(const base::string16& display_name) {
    display_name_ = display_name;
  }

  void set_given_name(const base::string16& given_name) { given_name_ = given_name; }

  void set_display_email(const std::string& display_email) {
    display_email_ = display_email;
  }

  const UserImage& user_image() const { return user_image_; }

  void set_username_hash(const std::string& username_hash) {
    username_hash_ = username_hash;
  }

  void set_is_logged_in(bool is_logged_in) {
    is_logged_in_ = is_logged_in;
  }

  void set_is_active(bool is_active) {
    is_active_ = is_active;
  }

  void set_profile_is_created() {
    profile_is_created_ = true;
  }

  
  bool has_gaia_account() const;

 private:
  std::string email_;
  base::string16 display_name_;
  base::string16 given_name_;
  
  std::string display_email_;
  UserImage user_image_;
  OAuthTokenStatus oauth_token_status_;

  
  
  
  
  scoped_ptr<std::string> account_locale_;

  
  std::string username_hash_;

  
  
  int image_index_;

  
  bool image_is_stub_;

  
  bool image_is_loading_;

  
  bool is_logged_in_;

  
  bool is_active_;

  
  bool profile_is_created_;

  DISALLOW_COPY_AND_ASSIGN(User);
};

typedef std::vector<User*> UserList;

}  

#endif  

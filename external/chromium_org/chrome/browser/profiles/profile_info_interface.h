// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_PROFILE_INFO_INTERFACE_H_
#define CHROME_BROWSER_PROFILES_PROFILE_INFO_INTERFACE_H_

#include "base/files/file_path.h"
#include "base/strings/string16.h"

namespace gfx {
class Image;
}

class ProfileInfoInterface {
 public:
  virtual size_t GetNumberOfProfiles() const = 0;

  virtual size_t GetIndexOfProfileWithPath(
      const base::FilePath& profile_path) const = 0;

  virtual base::string16 GetNameOfProfileAtIndex(size_t index) const = 0;

  virtual base::string16 GetShortcutNameOfProfileAtIndex(
      size_t index) const = 0;

  virtual base::FilePath GetPathOfProfileAtIndex(size_t index) const = 0;

  virtual base::string16 GetUserNameOfProfileAtIndex(size_t index) const = 0;

  virtual const gfx::Image& GetAvatarIconOfProfileAtIndex(
      size_t index) const = 0;

  virtual std::string GetLocalAuthCredentialsOfProfileAtIndex(
      size_t index) const = 0;

  
  
  virtual bool GetBackgroundStatusOfProfileAtIndex(
      size_t index) const = 0;

  virtual base::string16 GetGAIANameOfProfileAtIndex(size_t index) const = 0;

  virtual base::string16 GetGAIAGivenNameOfProfileAtIndex(
      size_t index) const = 0;

  
  virtual bool IsUsingGAIANameOfProfileAtIndex(size_t index) const = 0;

  virtual const gfx::Image* GetGAIAPictureOfProfileAtIndex(
      size_t index) const = 0;

  
  virtual bool IsUsingGAIAPictureOfProfileAtIndex(size_t index) const = 0;

  virtual bool ProfileIsManagedAtIndex(size_t index) const = 0;

  virtual std::string GetManagedUserIdOfProfileAtIndex(size_t index) const = 0;

  
  virtual bool ProfileIsSigninRequiredAtIndex(size_t index) const = 0;

  
  virtual bool ProfileIsEphemeralAtIndex(size_t index) const = 0;

 protected:
  virtual ~ProfileInfoInterface() {}
};

#endif  

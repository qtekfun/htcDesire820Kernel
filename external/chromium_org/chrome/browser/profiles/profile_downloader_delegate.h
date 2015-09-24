// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_PROFILE_DOWNLOADER_DELEGATE_H_
#define CHROME_BROWSER_PROFILES_PROFILE_DOWNLOADER_DELEGATE_H_

#include <string>

#include "base/basictypes.h"
#include "base/strings/string16.h"

class Profile;
class ProfileDownloader;

class ProfileDownloaderDelegate {
 public:
  
  enum FailureReason {
    TOKEN_ERROR,         
    NETWORK_ERROR,       
    SERVICE_ERROR,       
    IMAGE_DECODE_FAILED  
  };

  virtual ~ProfileDownloaderDelegate() {}

  
  virtual bool NeedsProfilePicture() const = 0;

  
  
  virtual int GetDesiredImageSideLength() const = 0;

  
  
  
  virtual std::string GetCachedPictureURL() const = 0;

  
  virtual Profile* GetBrowserProfile() = 0;

  
  
  virtual void OnProfileDownloadSuccess(ProfileDownloader* downloader) = 0;

  
  virtual void OnProfileDownloadFailure(
      ProfileDownloader* downloader,
      ProfileDownloaderDelegate::FailureReason reason) = 0;
};

#endif  

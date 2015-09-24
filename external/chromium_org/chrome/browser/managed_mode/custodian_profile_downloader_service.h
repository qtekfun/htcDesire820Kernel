// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MANAGED_MODE_CUSTODIAN_PROFILE_DOWNLOADER_SERVICE_H_
#define CHROME_BROWSER_MANAGED_MODE_CUSTODIAN_PROFILE_DOWNLOADER_SERVICE_H_

#include "base/callback.h"
#include "chrome/browser/profiles/profile_downloader.h"
#include "chrome/browser/profiles/profile_downloader_delegate.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"

class CustodianProfileDownloaderService : public BrowserContextKeyedService,
                                          public ProfileDownloaderDelegate {
 public:
  
  
  typedef base::Callback<void(const base::string16& )>
      DownloadProfileCallback;

  virtual ~CustodianProfileDownloaderService();

  
  virtual void Shutdown() OVERRIDE;

  
  
  
  
  
  void DownloadProfile(const DownloadProfileCallback& callback);

  
  virtual bool NeedsProfilePicture() const OVERRIDE;
  virtual int GetDesiredImageSideLength() const OVERRIDE;
  virtual std::string GetCachedPictureURL() const OVERRIDE;
  virtual Profile* GetBrowserProfile() OVERRIDE;
  virtual void OnProfileDownloadSuccess(ProfileDownloader* downloader) OVERRIDE;
  virtual void OnProfileDownloadFailure(
      ProfileDownloader* downloader,
      ProfileDownloaderDelegate::FailureReason reason) OVERRIDE;

 private:
  friend class CustodianProfileDownloaderServiceFactory;
  
  
  explicit CustodianProfileDownloaderService(Profile* custodian_profile);

  scoped_ptr<ProfileDownloader> profile_downloader_;
  DownloadProfileCallback download_callback_;

  
  Profile* custodian_profile_;

  std::string last_downloaded_profile_email_;
  std::string in_progress_profile_email_;
};

#endif  

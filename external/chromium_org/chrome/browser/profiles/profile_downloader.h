// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_PROFILE_DOWNLOADER_H_
#define CHROME_BROWSER_PROFILES_PROFILE_DOWNLOADER_H_

#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/image_decoder.h"
#include "google_apis/gaia/oauth2_token_service.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "url/gurl.h"

class ProfileDownloaderDelegate;
class OAuth2AccessTokenFetcher;

namespace net {
class URLFetcher;
}  

class ProfileDownloader : public net::URLFetcherDelegate,
                          public ImageDecoder::Delegate,
                          public OAuth2TokenService::Observer,
                          public OAuth2TokenService::Consumer {
 public:
  enum PictureStatus {
    PICTURE_SUCCESS,
    PICTURE_FAILED,
    PICTURE_DEFAULT,
    PICTURE_CACHED,
  };

  explicit ProfileDownloader(ProfileDownloaderDelegate* delegate);
  virtual ~ProfileDownloader();

  
  
  
  virtual void Start();

  
  
  
  virtual void StartForAccount(const std::string& account_id);

  
  
  virtual base::string16 GetProfileFullName() const;

  
  
  virtual base::string16 GetProfileGivenName() const;

  
  virtual std::string GetProfileLocale() const;

  
  
  
  virtual SkBitmap GetProfilePicture() const;

  
  virtual PictureStatus GetProfilePictureStatus() const;

  
  
  
  virtual std::string GetProfilePictureURL() const;

 private:
  friend class ProfileDownloaderTest;
  FRIEND_TEST_ALL_PREFIXES(ProfileDownloaderTest, ParseData);
  FRIEND_TEST_ALL_PREFIXES(ProfileDownloaderTest, DefaultURL);

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  virtual void OnImageDecoded(const ImageDecoder* decoder,
                              const SkBitmap& decoded_image) OVERRIDE;
  virtual void OnDecodeImageFailed(const ImageDecoder* decoder) OVERRIDE;

  
  virtual void OnRefreshTokenAvailable(const std::string& account_id) OVERRIDE;

  
  virtual void OnGetTokenSuccess(const OAuth2TokenService::Request* request,
                                 const std::string& access_token,
                                 const base::Time& expiration_time) OVERRIDE;
  virtual void OnGetTokenFailure(const OAuth2TokenService::Request* request,
                                 const GoogleServiceAuthError& error) OVERRIDE;

  
  
  
  static bool ParseProfileJSON(const std::string& data,
                               base::string16* full_name,
                               base::string16* given_name,
                               std::string* url,
                               int image_size,
                               std::string* profile_locale);
  
  static bool IsDefaultProfileImageURL(const std::string& url);

  
  void StartFetchingImage();

  
  const char* GetAuthorizationHeader() const;

  
  
  void StartFetchingOAuth2AccessToken();

  ProfileDownloaderDelegate* delegate_;
  std::string account_id_;
  std::string auth_token_;
  scoped_ptr<net::URLFetcher> user_entry_fetcher_;
  scoped_ptr<net::URLFetcher> profile_image_fetcher_;
  scoped_ptr<OAuth2TokenService::Request> oauth2_access_token_request_;
  base::string16 profile_full_name_;
  base::string16 profile_given_name_;
  std::string profile_locale_;
  SkBitmap profile_picture_;
  PictureStatus picture_status_;
  std::string picture_url_;

  DISALLOW_COPY_AND_ASSIGN(ProfileDownloader);
};

#endif  

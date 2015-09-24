// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_USER_IMAGE_DOWNLOADER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_USER_IMAGE_DOWNLOADER_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/chromeos/login/image_decoder.h"
#include "chrome/common/net/url_fetcher.h"

class ListValue;

namespace chromeos {

class UserImageDownloader : public URLFetcher::Delegate,
                            public ImageDecoder::Delegate {
 public:
  
  
  
  
  UserImageDownloader(const std::string& username,
                      const std::string& auth_token);

 private:
  
  ~UserImageDownloader();

  
  virtual void OnURLFetchComplete(const URLFetcher* source,
                                  const GURL& url,
                                  const net::URLRequestStatus& status,
                                  int response_code,
                                  const ResponseCookies& cookies,
                                  const std::string& data);

  
  virtual void OnImageDecoded(const SkBitmap& decoded_image);

  
  
  
  bool GetImageURL(const std::string& json_data, GURL* image_url) const;

  
  
  
  bool GetImageURLFromEntries(ListValue* entry_list, GURL* image_url) const;

  
  
  bool IsUserEntry(ListValue* email_list) const;

  
  
  bool GetImageURLFromLinks(ListValue* link_list, GURL* image_url) const;

  
  
  void SaveImageAsPNG(const std::string& filename, const SkBitmap& image);

  
  scoped_ptr<URLFetcher> profile_fetcher_;

  
  std::string username_;

  
  std::string auth_token_;

  DISALLOW_COPY_AND_ASSIGN(UserImageDownloader);
};

}  

#endif  


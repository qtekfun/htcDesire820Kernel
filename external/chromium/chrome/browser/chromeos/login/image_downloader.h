// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_IMAGE_DOWNLOADER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_IMAGE_DOWNLOADER_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/chromeos/login/image_decoder.h"
#include "chrome/common/net/url_fetcher.h"
#include "googleurl/src/gurl.h"

namespace chromeos {

class ImageDownloader : public URLFetcher::Delegate {
 public:
  
  
  ImageDownloader(ImageDecoder::Delegate* delegate,
                  const GURL& image_url,
                  const std::string& auth_token);
  virtual ~ImageDownloader() {}

 private:
  
  virtual void OnURLFetchComplete(const URLFetcher* source,
                                  const GURL& url,
                                  const net::URLRequestStatus& status,
                                  int response_code,
                                  const ResponseCookies& cookies,
                                  const std::string& data);

  ImageDecoder::Delegate* delegate_;
  scoped_ptr<URLFetcher> image_fetcher_;

  DISALLOW_COPY_AND_ASSIGN(ImageDownloader);
};

}  

#endif  

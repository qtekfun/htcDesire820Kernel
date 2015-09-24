// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_SYNC_NOTIFIER_NOTIFICATION_BITMAP_FETCHER_H_
#define CHROME_BROWSER_NOTIFICATIONS_SYNC_NOTIFIER_NOTIFICATION_BITMAP_FETCHER_H_

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/image_decoder.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "url/gurl.h"

namespace net {
class URLFetcher;
}  

class Profile;

namespace notifier {

class NotificationBitmapFetcherDelegate {
 public:
  
  
  
  virtual void OnFetchComplete(const GURL url, const SkBitmap* bitmap) = 0;

 protected:
  virtual ~NotificationBitmapFetcherDelegate() {}
};

class NotificationBitmapFetcher
    : public net::URLFetcherDelegate,
      public ImageDecoder::Delegate {
 public:
  NotificationBitmapFetcher(
      const GURL& url,
      NotificationBitmapFetcherDelegate* delegate);
  virtual ~NotificationBitmapFetcher();

  GURL url() const { return url_; }

  
  
  void Start(Profile* profile);

  

  
  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  
  
  virtual void OnURLFetchDownloadProgress(const net::URLFetcher* source,
                                          int64 current, int64 total) OVERRIDE;

  

  
  
  
  virtual void OnImageDecoded(const ImageDecoder* decoder,
                              const SkBitmap& decoded_image) OVERRIDE;

  
  virtual void OnDecodeImageFailed(const ImageDecoder* decoder) OVERRIDE;

 private:
  scoped_ptr<net::URLFetcher> url_fetcher_;
  scoped_refptr<ImageDecoder> image_decoder_;
  const GURL url_;
  scoped_ptr<SkBitmap> bitmap_;
  NotificationBitmapFetcherDelegate* const delegate_;

  DISALLOW_COPY_AND_ASSIGN(NotificationBitmapFetcher);
};

}  

#endif  

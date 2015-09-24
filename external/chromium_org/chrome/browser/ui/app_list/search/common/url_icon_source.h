// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_LIST_SEARCH_COMMON_URL_ICON_SOURCE_H_
#define CHROME_BROWSER_UI_APP_LIST_SEARCH_COMMON_URL_ICON_SOURCE_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/image_decoder.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/image/image_skia_source.h"
#include "url/gurl.h"

namespace net {
class URLFetcher;
class URLRequestContextGetter;
}

namespace app_list {

class UrlIconSource : public gfx::ImageSkiaSource,
                      public net::URLFetcherDelegate,
                      public ImageDecoder::Delegate {
 public:
  typedef base::Closure IconLoadedCallback;

  
  
  UrlIconSource(const IconLoadedCallback& icon_loaded_callback,
                net::URLRequestContextGetter* context_getter,
                const GURL& icon_url,
                int icon_size,
                int default_icon_resource_id);
  virtual ~UrlIconSource();

 private:
  
  
  void StartIconFetch();

  
  virtual gfx::ImageSkiaRep GetImageForScale(float scale) OVERRIDE;

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  virtual void OnImageDecoded(const ImageDecoder* decoder,
                              const SkBitmap& decoded_image) OVERRIDE;
  virtual void OnDecodeImageFailed(const ImageDecoder* decoder) OVERRIDE;

  IconLoadedCallback icon_loaded_callback_;
  net::URLRequestContextGetter* context_getter_;
  const GURL icon_url_;
  const int icon_size_;
  const int default_icon_resource_id_;

  bool icon_fetch_attempted_;
  scoped_ptr<net::URLFetcher> icon_fetcher_;

  scoped_refptr<ImageDecoder> image_decoder_;

  gfx::ImageSkia icon_;

  DISALLOW_COPY_AND_ASSIGN(UrlIconSource);
};

}  

#endif  

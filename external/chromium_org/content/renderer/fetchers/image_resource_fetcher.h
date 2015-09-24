// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_FETCHERS_IMAGE_RESOURCE_FETCHER_H_
#define CONTENT_RENDERER_FETCHERS_IMAGE_RESOURCE_FETCHER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "third_party/WebKit/public/platform/WebURLRequest.h"
#include "url/gurl.h"

class SkBitmap;

namespace blink {
class WebFrame;
class WebURLResponse;
}

namespace content {

class ResourceFetcher;

class ImageResourceFetcher {
 public:
  typedef base::Callback<void(ImageResourceFetcher*, const SkBitmap&)> Callback;

  ImageResourceFetcher(
      const GURL& image_url,
      blink::WebFrame* frame,
      int id,
      int image_size,
      blink::WebURLRequest::TargetType target_type,
      const Callback& callback);

  virtual ~ImageResourceFetcher();

  
  const GURL& image_url() const { return image_url_; }

  
  int id() const { return id_; }

 private:
  
  void OnURLFetchComplete(const blink::WebURLResponse& response,
                          const std::string& data);

  Callback callback_;

  
  const int id_;

  
  const GURL image_url_;

  
  
  
  const int image_size_;

  
  scoped_ptr<ResourceFetcher> fetcher_;

  DISALLOW_COPY_AND_ASSIGN(ImageResourceFetcher);
};

}  

#endif  

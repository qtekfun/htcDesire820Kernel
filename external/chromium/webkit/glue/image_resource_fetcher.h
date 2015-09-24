// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_IMAGE_RESOURCE_FETCHER_H_
#define WEBKIT_GLUE_IMAGE_RESOURCE_FETCHER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "webkit/glue/resource_fetcher.h"

class SkBitmap;

namespace webkit_glue {

class ImageResourceFetcher {
 public:
  typedef Callback2<ImageResourceFetcher*, const SkBitmap&>::Type Callback;

  ImageResourceFetcher(const GURL& image_url,
                       WebKit::WebFrame* frame,
                       int id,
                       int image_size,
                       WebKit::WebURLRequest::TargetType target_type,
                       Callback* callback);

  virtual ~ImageResourceFetcher();

  
  const GURL& image_url() const { return image_url_; }

  
  int id() const { return id_; }

 private:
  
  void OnURLFetchComplete(const WebKit::WebURLResponse& response,
                          const std::string& data);

  scoped_ptr<Callback> callback_;

  
  const int id_;

  
  const GURL image_url_;

  
  
  
  const int image_size_;

  
  scoped_ptr<ResourceFetcher> fetcher_;

  DISALLOW_COPY_AND_ASSIGN(ImageResourceFetcher);
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_FETCHERS_MULTI_RESOLUTION_IMAGE_RESOURCE_FETCHER_H_
#define CONTENT_RENDERER_FETCHERS_MULTI_RESOLUTION_IMAGE_RESOURCE_FETCHER_H_

#include <string>
#include <vector>

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

class MultiResolutionImageResourceFetcher {
 public:
  typedef base::Callback<void(MultiResolutionImageResourceFetcher*,
                              const std::vector<SkBitmap>&)> Callback;

  MultiResolutionImageResourceFetcher(
      const GURL& image_url,
      blink::WebFrame* frame,
      int id,
      blink::WebURLRequest::TargetType target_type,
      const Callback& callback);

  virtual ~MultiResolutionImageResourceFetcher();

  
  const GURL& image_url() const { return image_url_; }

  
  int id() const { return id_; }

  
  int http_status_code() const { return http_status_code_; }

 private:
  
  void OnURLFetchComplete(const blink::WebURLResponse& response,
                          const std::string& data);

  Callback callback_;

  
  const int id_;

  
  int http_status_code_;

  
  const GURL image_url_;

  
  scoped_ptr<ResourceFetcher> fetcher_;

  DISALLOW_COPY_AND_ASSIGN(MultiResolutionImageResourceFetcher);
};

}  

#endif  

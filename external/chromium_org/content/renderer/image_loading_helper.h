// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_IMAGE_LOADING_HELPER_H_
#define CONTENT_RENDERER_IMAGE_LOADING_HELPER_H_

#include <string>
#include <vector>

#include "base/memory/scoped_vector.h"
#include "content/public/renderer/render_view_observer.h"
#include "url/gurl.h"

class SkBitmap;

namespace content {

class MultiResolutionImageResourceFetcher;

class ImageLoadingHelper : public RenderViewObserver {
 public:
  explicit ImageLoadingHelper(RenderView* render_view);

 private:
  virtual ~ImageLoadingHelper();

  
  void OnDownloadImage(int id,
                       const GURL& image_url,
                       bool is_favicon,
                       uint32_t max_image_size);

  
  
  
  
  
  
  
  
  
  
  bool DownloadImage(int id,
                     const GURL& image_url,
                     bool is_favicon,
                     uint32_t max_image_size);

  
  
  
  void DidDownloadImage(
      uint32_t max_image_size,
      MultiResolutionImageResourceFetcher* fetcher,
      const std::vector<SkBitmap>& images);

  
  SkBitmap ImageFromDataUrl(const GURL&) const;

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  typedef ScopedVector<MultiResolutionImageResourceFetcher>
      ImageResourceFetcherList;

  
  ImageResourceFetcherList image_fetchers_;

  DISALLOW_COPY_AND_ASSIGN(ImageLoadingHelper);
};

}  

#endif  


// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_IMAGE_LOADER_H_
#define CHROME_BROWSER_EXTENSIONS_IMAGE_LOADER_H_

#include <set>

#include "base/callback_forward.h"
#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "extensions/common/extension_resource.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/base/layout.h"
#include "ui/gfx/size.h"

namespace content {
class BrowserContext;
}

namespace gfx {
class Image;
}

namespace extensions {

class Extension;

typedef base::Callback<void(const gfx::Image&)> ImageLoaderCallback;

class ImageLoader : public BrowserContextKeyedService {
 public:
  
  
  struct ImageRepresentation {
    
    
    enum ResizeCondition {
      RESIZE_WHEN_LARGER,
      ALWAYS_RESIZE,
    };

    ImageRepresentation(const ExtensionResource& resource,
                        ResizeCondition resize_condition,
                        const gfx::Size& desired_size,
                        ui::ScaleFactor scale_factor);
    ~ImageRepresentation();

    
    ExtensionResource resource;

    ResizeCondition resize_condition;

    
    
    gfx::Size desired_size;

    
    ui::ScaleFactor scale_factor;
  };

  struct LoadResult;

  
  
  static ImageLoader* Get(content::BrowserContext* context);

  ImageLoader();
  virtual ~ImageLoader();

  
  
  
  
  static bool IsComponentExtensionResource(
      const base::FilePath& extension_path,
      const base::FilePath& resource_path,
      int* resource_id);

  
  
  
  
  
  
  void LoadImageAsync(const extensions::Extension* extension,
                      const ExtensionResource& resource,
                      const gfx::Size& max_size,
                      const ImageLoaderCallback& callback);

  
  
  
  void LoadImagesAsync(const extensions::Extension* extension,
                       const std::vector<ImageRepresentation>& info_list,
                       const ImageLoaderCallback& callback);

 private:
  void ReplyBack(const ImageLoaderCallback& callback,
                 const std::vector<LoadResult>& load_result);

  base::WeakPtrFactory<ImageLoader> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ImageLoader);
};

}  

#endif  

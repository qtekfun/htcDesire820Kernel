// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SKIA_EXT_IMAGE_OPERATIONS_H_
#define SKIA_EXT_IMAGE_OPERATIONS_H_

#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkTypes.h"

struct SkIRect;

namespace skia {

class SK_API ImageOperations {
 public:
  enum ResizeMethod {
    
    
    
    
    
    
    
    

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    RESIZE_GOOD,

    
    
    
    
    
    
    
    
    RESIZE_BETTER,

    
    RESIZE_BEST,

    
    
    

    
    
    
    
    
    RESIZE_BOX,

    
    
    
    RESIZE_HAMMING1,

    
    
    
    RESIZE_LANCZOS2,

    
    
    RESIZE_LANCZOS3,

    
    
    RESIZE_SUBPIXEL,

    
    RESIZE_FIRST_QUALITY_METHOD = RESIZE_GOOD,
    RESIZE_LAST_QUALITY_METHOD = RESIZE_BEST,
    RESIZE_FIRST_ALGORITHM_METHOD = RESIZE_BOX,
    RESIZE_LAST_ALGORITHM_METHOD = RESIZE_SUBPIXEL,
  };

  
  
  
  
  
  
  
  
  static SkBitmap Resize(const SkBitmap& source,
                         ResizeMethod method,
                         int dest_width, int dest_height,
                         const SkIRect& dest_subset,
                         SkBitmap::Allocator* allocator = NULL);

  
  
  static SkBitmap Resize(const SkBitmap& source,
                         ResizeMethod method,
                         int dest_width, int dest_height,
                         SkBitmap::Allocator* allocator = NULL);

 private:
  ImageOperations();  

  
  static SkBitmap ResizeBasic(const SkBitmap& source,
                              ResizeMethod method,
                              int dest_width, int dest_height,
                              const SkIRect& dest_subset,
                              SkBitmap::Allocator* allocator = NULL);

  
  static SkBitmap ResizeSubpixel(const SkBitmap& source,
                                 int dest_width, int dest_height,
                                 const SkIRect& dest_subset,
                                 SkBitmap::Allocator* allocator = NULL);
};

}  

#endif  

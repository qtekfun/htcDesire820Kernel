// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SKIA_EXT_SKIA_UTILS_IOS_H_
#define SKIA_EXT_SKIA_UTILS_IOS_H_

#include <CoreGraphics/CoreGraphics.h>
#include <vector>

#include "third_party/skia/include/core/SkBitmap.h"

#ifdef __OBJC__
@class UIImage;
@class NSData;
#else
class UIImage;
class NSData;
#endif

namespace gfx {

SK_API SkBitmap CGImageToSkBitmap(CGImageRef image,
                                  CGSize size,
                                  bool is_opaque);

SK_API UIImage* SkBitmapToUIImageWithColorSpace(const SkBitmap& skia_bitmap,
                                                CGFloat scale,
                                                CGColorSpaceRef color_space);

SK_API std::vector<SkBitmap> ImageDataToSkBitmaps(NSData* image_data);

}  

#endif  

// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_IMAGE_IMAGE_SKIA_UTIL_IOS_H_
#define UI_GFX_IMAGE_IMAGE_SKIA_UTIL_IOS_H_

#include "ui/gfx/gfx_export.h"

#ifdef __OBJC__
@class UIImage;
#else
class UIImage;
#endif

namespace gfx {
class ImageSkia;
class ImageSkiaRep;

GFX_EXPORT gfx::ImageSkia ImageSkiaFromUIImage(UIImage* image);

GFX_EXPORT gfx::ImageSkiaRep ImageSkiaRepOfScaleFromUIImage(
    UIImage* image,
    float scale);

GFX_EXPORT UIImage* UIImageFromImageSkia(const gfx::ImageSkia& image_skia);

GFX_EXPORT UIImage* UIImageFromImageSkiaRep(
    const gfx::ImageSkiaRep& image_skia_rep);

}  

#endif  

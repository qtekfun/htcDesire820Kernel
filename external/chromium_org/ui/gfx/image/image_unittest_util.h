// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef UI_GFX_IMAGE_IMAGE_UNITTEST_UTIL_H_
#define UI_GFX_IMAGE_IMAGE_UNITTEST_UTIL_H_

#include "ui/gfx/image/image.h"
#include "third_party/skia/include/core/SkColor.h"

namespace gfx {
namespace test {

#if defined(OS_IOS)
typedef UIImage* PlatformImage;
#elif defined(OS_MACOSX)
typedef NSImage* PlatformImage;
#elif defined(TOOLKIT_GTK)
typedef GdkPixbuf* PlatformImage;
#else
typedef gfx::ImageSkia PlatformImage;
#endif

std::vector<float> Get1xAnd2xScales();

const SkBitmap CreateBitmap(int width, int height);

gfx::ImageSkia CreateImageSkia(int width, int height);

scoped_refptr<base::RefCountedMemory> CreatePNGBytes(int edge_size);

gfx::Image CreateImage();
gfx::Image CreateImage(int width, int height);

bool IsEqual(const gfx::Image& image1, const gfx::Image& image2);

bool IsEqual(const SkBitmap& bitmap1, const SkBitmap& bitmap2);

bool IsEqual(const scoped_refptr<base::RefCountedMemory>& bytes,
             const SkBitmap& bitmap);

void CheckImageIndicatesPNGDecodeFailure(const gfx::Image& image);

bool ImageSkiaStructureMatches(
    const gfx::ImageSkia& image_skia,
    int width,
    int height,
    const std::vector<float>& scale_factors);

bool IsEmpty(const gfx::Image& image);

PlatformImage CreatePlatformImage();

gfx::Image::RepresentationType GetPlatformRepresentationType();

PlatformImage ToPlatformType(const gfx::Image& image);
PlatformImage CopyPlatformType(const gfx::Image& image);

SkColor GetPlatformImageColor(PlatformImage image, int x, int y);
void CheckColors(SkColor color1, SkColor color2);
void CheckIsTransparent(SkColor color);

bool IsPlatformImageValid(PlatformImage image);

bool PlatformImagesEqual(PlatformImage image1, PlatformImage image2);

}  
}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SKIA_EXT_SKIA_UTILS_MAC_H_
#define SKIA_EXT_SKIA_UTILS_MAC_H_

#include <ApplicationServices/ApplicationServices.h>
#include <vector>

#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkColor.h"

struct SkIRect;
struct SkPoint;
struct SkRect;
class SkCanvas;
class SkMatrix;
#ifdef __LP64__
typedef CGSize NSSize;
#else
typedef struct _NSSize NSSize;
#endif

#ifdef __OBJC__
@class NSBitmapImageRep;
@class NSImage;
@class NSImageRep;
@class NSColor;
#else
class NSBitmapImageRep;
class NSImage;
class NSImageRep;
class NSColor;
#endif

namespace gfx {

inline const CGPoint& SkPointToCGPoint(const SkPoint& point) {
  return reinterpret_cast<const CGPoint&>(point);
}

inline const SkPoint& CGPointToSkPoint(const CGPoint& point) {
  return reinterpret_cast<const SkPoint&>(point);
}

SK_API CGAffineTransform SkMatrixToCGAffineTransform(const SkMatrix& matrix);

SK_API SkRect CGRectToSkRect(const CGRect& rect);

CGRect SkIRectToCGRect(const SkIRect& rect);
CGRect SkRectToCGRect(const SkRect& rect);

SK_API SkColor CGColorRefToSkColor(CGColorRef color);

SK_API CGColorRef CGColorCreateFromSkColor(SkColor color);

SK_API SkColor NSDeviceColorToSkColor(NSColor* color);

SK_API NSColor* SkColorToCalibratedNSColor(SkColor color);
SK_API NSColor* SkColorToDeviceNSColor(SkColor color);
SK_API NSColor* SkColorToSRGBNSColor(SkColor color);

SK_API SkBitmap CGImageToSkBitmap(CGImageRef image);

SK_API SkBitmap NSImageToSkBitmapWithColorSpace(NSImage* image,
                                                bool is_opaque,
                                                CGColorSpaceRef color_space);

SK_API SkBitmap NSImageRepToSkBitmapWithColorSpace(NSImageRep* image,
                                                   NSSize size,
                                                   bool is_opaque,
                                                   CGColorSpaceRef colorspace);

SK_API NSBitmapImageRep* SkBitmapToNSBitmapImageRep(const SkBitmap& image);

SK_API NSBitmapImageRep* SkBitmapToNSBitmapImageRepWithColorSpace(
    const SkBitmap& skiaBitmap,
    CGColorSpaceRef colorSpace);

SK_API NSImage* SkBitmapToNSImageWithColorSpace(const SkBitmap& icon,
                                                CGColorSpaceRef colorSpace);

SK_API NSImage* SkBitmapToNSImage(const SkBitmap& icon);

class SK_API SkiaBitLocker {
 public:
  explicit SkiaBitLocker(SkCanvas* canvas);
  ~SkiaBitLocker();
  CGContextRef cgContext();

 private:
  void releaseIfNeeded();
  SkCanvas* canvas_;
  CGContextRef cgContext_;
  SkBitmap bitmap_;
  bool useDeviceBits_;
};


}  

#endif  

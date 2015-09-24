// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SKIA_EXT_PLATFORM_CANVAS_H_
#define SKIA_EXT_PLATFORM_CANVAS_H_

#include "base/basictypes.h"
#include "skia/ext/platform_device.h"
#include "skia/ext/refptr.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkCanvas.h"
#include "third_party/skia/include/core/SkPixelRef.h"

namespace skia {

typedef SkCanvas PlatformCanvas;

enum OnFailureType {
  CRASH_ON_FAILURE,
  RETURN_NULL_ON_FAILURE
};

#if defined(WIN32)
  
  
  SK_API SkCanvas* CreatePlatformCanvas(int width,
                                        int height,
                                        bool is_opaque,
                                        HANDLE shared_section,
                                        OnFailureType failure_type);
#elif defined(__APPLE__)
  SK_API SkCanvas* CreatePlatformCanvas(CGContextRef context,
                                        int width,
                                        int height,
                                        bool is_opaque,
                                        OnFailureType failure_type);

  SK_API SkCanvas* CreatePlatformCanvas(int width,
                                        int height,
                                        bool is_opaque,
                                        uint8_t* context,
                                        OnFailureType failure_type);
#elif defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) || \
      defined(__sun) || defined(ANDROID)
  

  
  
  SK_API SkCanvas* CreatePlatformCanvas(int width,
                                        int height,
                                        bool is_opaque,
                                        uint8_t* data,
                                        OnFailureType failure_type);
#endif

static inline SkCanvas* CreatePlatformCanvas(int width,
                                             int height,
                                             bool is_opaque) {
  return CreatePlatformCanvas(width, height, is_opaque, 0, CRASH_ON_FAILURE);
}

SK_API SkCanvas* CreateCanvas(const skia::RefPtr<SkBaseDevice>& device,
                              OnFailureType failure_type);

static inline SkCanvas* CreateBitmapCanvas(int width,
                                           int height,
                                           bool is_opaque) {
  return CreatePlatformCanvas(width, height, is_opaque, 0, CRASH_ON_FAILURE);
}

static inline SkCanvas* TryCreateBitmapCanvas(int width,
                                              int height,
                                              bool is_opaque) {
  return CreatePlatformCanvas(width, height, is_opaque, 0,
                              RETURN_NULL_ON_FAILURE);
}

SK_API size_t PlatformCanvasStrideForWidth(unsigned width);

SK_API SkBaseDevice* GetTopDevice(const SkCanvas& canvas);

SK_API bool SupportsPlatformPaint(const SkCanvas* canvas);

SK_API void DrawToNativeContext(SkCanvas* canvas,
                                PlatformSurface context,
                                int x,
                                int y,
                                const PlatformRect* src_rect);

SK_API void MakeOpaque(SkCanvas* canvas, int x, int y, int width, int height);

SK_API PlatformSurface BeginPlatformPaint(SkCanvas* canvas);
SK_API void EndPlatformPaint(SkCanvas* canvas);

class SK_API ScopedPlatformPaint {
 public:
  explicit ScopedPlatformPaint(SkCanvas* canvas) : canvas_(canvas) {
    platform_surface_ = BeginPlatformPaint(canvas);
  }
  ~ScopedPlatformPaint() { EndPlatformPaint(canvas_); }

  
  PlatformSurface GetPlatformSurface() { return platform_surface_; }
 private:
  SkCanvas* canvas_;
  PlatformSurface platform_surface_;

  
  ScopedPlatformPaint(const ScopedPlatformPaint&);
  ScopedPlatformPaint& operator=(const ScopedPlatformPaint&);
};

class SK_API PlatformBitmap {
 public:
  PlatformBitmap();
  ~PlatformBitmap();

  
  bool Allocate(int width, int height, bool is_opaque);

  
  PlatformSurface GetSurface() { return surface_; }

  
  
  
  
  
  
  const SkBitmap& GetBitmap() { return bitmap_; }

 private:
  SkBitmap bitmap_;
  PlatformSurface surface_;  
  intptr_t platform_extra_;  

  DISALLOW_COPY_AND_ASSIGN(PlatformBitmap);
};

}  

#endif  

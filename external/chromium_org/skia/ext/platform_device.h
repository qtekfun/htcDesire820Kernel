// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SKIA_EXT_PLATFORM_DEVICE_H_
#define SKIA_EXT_PLATFORM_DEVICE_H_

#include "build/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#include <vector>
#endif

#include "third_party/skia/include/core/SkColor.h"
#include "third_party/skia/include/core/SkBitmapDevice.h"
#include "third_party/skia/include/core/SkPreConfig.h"

class SkMatrix;
class SkMetaData;
class SkPath;
class SkRegion;

#if defined(USE_CAIRO)
typedef struct _cairo cairo_t;
typedef struct _cairo_rectangle cairo_rectangle_t;
#elif defined(OS_MACOSX)
typedef struct CGContext* CGContextRef;
typedef struct CGRect CGRect;
#endif

namespace skia {

class PlatformDevice;

#if defined(OS_WIN)
typedef HDC PlatformSurface;
typedef RECT PlatformRect;
#elif defined(USE_CAIRO)
typedef cairo_t* PlatformSurface;
typedef cairo_rectangle_t PlatformRect;
#elif defined(OS_MACOSX)
typedef CGContextRef PlatformSurface;
typedef CGRect PlatformRect;
#else
typedef void* PlatformSurface;
typedef SkIRect* PlatformRect;
#endif


SK_API void SetPlatformDevice(SkBaseDevice* device,
                              PlatformDevice* platform_device);
SK_API PlatformDevice* GetPlatformDevice(SkBaseDevice* device);


#if defined(OS_WIN)
SK_API void InitializeDC(HDC context);
#elif defined(OS_MACOSX)
SK_API CGContextRef GetBitmapContext(SkBaseDevice* device);
#endif

SK_API SkMetaData& getMetaData(const SkCanvas& canvas);
SK_API void SetIsDraftMode(const SkCanvas& canvas, bool draft_mode);
SK_API bool IsDraftMode(const SkCanvas& canvas);

#if defined(OS_MACOSX) || defined(OS_WIN)
SK_API void SetIsPreviewMetafile(const SkCanvas& canvas, bool is_preview);
SK_API bool IsPreviewMetafile(const SkCanvas& canvas);
#endif

class SK_API PlatformDevice {
 public:
  virtual ~PlatformDevice() {}

#if defined(OS_MACOSX)
  
  
  
  virtual CGContextRef GetBitmapContext() = 0;
#endif

  
  
  
  virtual PlatformSurface BeginPlatformPaint();

  
  virtual void EndPlatformPaint();

  
  
  
  
  
  virtual void DrawToNativeContext(PlatformSurface surface, int x, int y,
                                   const PlatformRect* src_rect) = 0;

  
  virtual bool SupportsPlatformPaint();

#if defined(OS_WIN)
  
  
  static bool LoadPathToDC(HDC context, const SkPath& path);

  
  static void LoadClippingRegionToDC(HDC context, const SkRegion& region,
                                     const SkMatrix& transformation);
#elif defined(OS_MACOSX)
  
  
  static void LoadPathToCGContext(CGContextRef context, const SkPath& path);

  
  static void InitializeCGContext(CGContextRef context);

  
  static void LoadClippingRegionToCGContext(CGContextRef context,
                                            const SkRegion& region,
                                            const SkMatrix& transformation);
#endif

 protected:
#if defined(OS_WIN)
  
  struct CubicPoints {
    SkPoint p[4];
  };
  typedef std::vector<CubicPoints> CubicPath;
  typedef std::vector<CubicPath> CubicPaths;

  
  
  static void LoadTransformToDC(HDC dc, const SkMatrix& matrix);

  
  static bool SkPathToCubicPaths(CubicPaths* paths, const SkPath& skpath);
#elif defined(OS_MACOSX)
  
  static void LoadTransformToCGContext(CGContextRef context,
                                       const SkMatrix& matrix);
#endif
};

}  

#endif  

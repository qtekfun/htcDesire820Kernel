// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SKIA_EXT_BITMAP_PLATFORM_DEVICE_CAIRO_H_
#define SKIA_EXT_BITMAP_PLATFORM_DEVICE_CAIRO_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "skia/ext/platform_device.h"

typedef struct _cairo_surface cairo_surface_t;


namespace skia {

class BitmapPlatformDevice : public SkBitmapDevice, public PlatformDevice {
 public:
  
  
  
  
  
  
  BitmapPlatformDevice(const SkBitmap& other, cairo_surface_t* surface);
  virtual ~BitmapPlatformDevice();

  
  
  
  static BitmapPlatformDevice* Create(int width, int height, bool is_opaque);

  
  
  
  static BitmapPlatformDevice* CreateAndClear(int width, int height,
                                              bool is_opaque);

  
  
  static BitmapPlatformDevice* Create(int width, int height, bool is_opaque,
                                      uint8_t* data);

  
  virtual void setMatrixClip(const SkMatrix& transform, const SkRegion& region,
                             const SkClipStack&) OVERRIDE;

  
  virtual cairo_t* BeginPlatformPaint() OVERRIDE;
  virtual void DrawToNativeContext(PlatformSurface surface, int x, int y,
                                   const PlatformRect* src_rect) OVERRIDE;

 protected:
  virtual SkBaseDevice* onCreateCompatibleDevice(SkBitmap::Config, int width,
                                                 int height, bool isOpaque,
                                                 Usage usage) OVERRIDE;

 private:
  static BitmapPlatformDevice* Create(int width, int height, bool is_opaque,
                                      cairo_surface_t* surface);

  
  
  
  void SetMatrixClip(const SkMatrix& transform, const SkRegion& region);

  
  void LoadConfig();

  
  cairo_t* cairo_;

  
  
  
  
  bool config_dirty_;

  
  
  SkMatrix transform_;

  
  SkRegion clip_region_;

  DISALLOW_COPY_AND_ASSIGN(BitmapPlatformDevice);
};

}  

#endif  

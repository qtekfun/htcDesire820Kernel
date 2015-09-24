// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SKIA_EXT_BITMAP_PLATFORM_DEVICE_SKIA_H_
#define SKIA_EXT_BITMAP_PLATFORM_DEVICE_SKIA_H_

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "skia/ext/platform_device.h"

namespace skia {

class BitmapPlatformDevice : public SkBitmapDevice, public PlatformDevice {
 public:
  
  
  
  static BitmapPlatformDevice* Create(int width, int height, bool is_opaque);

  
  
  static BitmapPlatformDevice* CreateAndClear(int width, int height,
                                              bool is_opaque);

  
  
  static BitmapPlatformDevice* Create(int width, int height, bool is_opaque,
                                      uint8_t* data);

  
  
  
  
  explicit BitmapPlatformDevice(const SkBitmap& other);
  virtual ~BitmapPlatformDevice();

  virtual PlatformSurface BeginPlatformPaint() OVERRIDE;
  virtual void DrawToNativeContext(PlatformSurface surface, int x, int y,
                                   const PlatformRect* src_rect) OVERRIDE;

 protected:
  virtual SkBaseDevice* onCreateCompatibleDevice(SkBitmap::Config, int width,
                                                 int height, bool isOpaque,
                                                 Usage usage) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(BitmapPlatformDevice);
};

}  

#endif  

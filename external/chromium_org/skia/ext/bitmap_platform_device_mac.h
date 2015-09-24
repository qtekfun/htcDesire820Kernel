// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SKIA_EXT_BITMAP_PLATFORM_DEVICE_MAC_H_
#define SKIA_EXT_BITMAP_PLATFORM_DEVICE_MAC_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "skia/ext/platform_device.h"
#include "skia/ext/refptr.h"

namespace skia {

class SK_API BitmapPlatformDevice : public SkBitmapDevice, public PlatformDevice {
 public:
  
  
  
  
  
  static BitmapPlatformDevice* Create(CGContextRef context,
                                      int width, int height,
                                      bool is_opaque);

  
  
  static BitmapPlatformDevice* CreateAndClear(int width, int height,
                                              bool is_opaque);

  
  
  static BitmapPlatformDevice* CreateWithData(uint8_t* data,
                                              int width, int height,
                                              bool is_opaque);

  virtual ~BitmapPlatformDevice();

  
  virtual CGContextRef GetBitmapContext() OVERRIDE;
  virtual void DrawToNativeContext(CGContextRef context, int x, int y,
                                   const CGRect* src_rect) OVERRIDE;

  
  virtual void setMatrixClip(const SkMatrix& transform, const SkRegion& region,
                             const SkClipStack&) OVERRIDE;

 protected:
  BitmapPlatformDevice(CGContextRef context,
                       const SkBitmap& bitmap);

  virtual SkBaseDevice* onCreateCompatibleDevice(SkBitmap::Config, int width,
                                                 int height, bool isOpaque,
                                                 Usage usage) OVERRIDE;

 private:
  void ReleaseBitmapContext();

  
  
  
  void SetMatrixClip(const SkMatrix& transform, const SkRegion& region);

  
  
  void LoadConfig();

  
  CGContextRef bitmap_context_;

  
  
  
  
  bool config_dirty_;

  
  
  SkMatrix transform_;

  
  SkRegion clip_region_;
  DISALLOW_COPY_AND_ASSIGN(BitmapPlatformDevice);
};

}  

#endif  

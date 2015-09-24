// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SKIA_EXT_BITMAP_PLATFORM_DEVICE_WIN_H_
#define SKIA_EXT_BITMAP_PLATFORM_DEVICE_WIN_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "skia/ext/platform_device.h"
#include "skia/ext/refptr.h"

namespace skia {

class SK_API BitmapPlatformDevice : public SkBitmapDevice, public PlatformDevice {
 public:
  
  
  
  
  
  
  
  
  static BitmapPlatformDevice* Create(int width, int height,
                                      bool is_opaque, HANDLE shared_section);

  
  
  static BitmapPlatformDevice* Create(int width, int height, bool is_opaque);

  
  
  static BitmapPlatformDevice* CreateAndClear(int width, int height,
                                              bool is_opaque);

  virtual ~BitmapPlatformDevice();

  
  
  
  virtual PlatformSurface BeginPlatformPaint() OVERRIDE;
  virtual void EndPlatformPaint() OVERRIDE;

  virtual void DrawToNativeContext(HDC dc, int x, int y,
                                   const RECT* src_rect) OVERRIDE;

  
  
  virtual void setMatrixClip(const SkMatrix& transform, const SkRegion& region,
                             const SkClipStack&) OVERRIDE;

 protected:
  
  
  
  virtual const SkBitmap& onAccessBitmap() OVERRIDE;

  virtual SkBaseDevice* onCreateCompatibleDevice(SkBitmap::Config, int width,
                                                 int height, bool isOpaque,
                                                 Usage usage) OVERRIDE;

 private:
  
  BitmapPlatformDevice(HBITMAP hbitmap, const SkBitmap& bitmap);

  
  
  
  HBITMAP hbitmap_;

  
  
  HBITMAP old_hbitmap_;

  
  HDC hdc_;

  
  
  
  
  bool config_dirty_;

  
  
  SkMatrix transform_;

  
  SkRegion clip_region_;

  
  HDC GetBitmapDC();
  void ReleaseBitmapDC();
  bool IsBitmapDCCreated() const;

  
  
  
  void SetMatrixClip(const SkMatrix& transform, const SkRegion& region);

  
  
  void LoadConfig();

#ifdef SK_DEBUG
  int begin_paint_count_;
#endif

  DISALLOW_COPY_AND_ASSIGN(BitmapPlatformDevice);
};

}  

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_OZONE_DRI_DRI_SURFACE_H_
#define UI_GFX_OZONE_DRI_DRI_SURFACE_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "ui/gfx/gfx_export.h"
#include "ui/gfx/skia_util.h"

class SkBitmapDevice;
class SkCanvas;

namespace gfx {

class DriSkBitmap;
class HardwareDisplayController;

class GFX_EXPORT DriSurface {
 public:
  DriSurface(HardwareDisplayController* controller);

  virtual ~DriSurface();

  
  
  
  
  bool Initialize();

  
  uint32_t GetFramebufferId() const;

  
  void SwapBuffers();

  
  SkCanvas* GetDrawableForWidget();

 private:
  friend class HardwareDisplayController;

  
  virtual DriSkBitmap* CreateBuffer();

  
  
  HardwareDisplayController* controller_;

  
  scoped_ptr<DriSkBitmap> bitmaps_[2];

  
  skia::RefPtr<SkBitmapDevice> skia_device_;

  
  skia::RefPtr<SkCanvas> skia_canvas_;

  
  int front_buffer_;

  DISALLOW_COPY_AND_ASSIGN(DriSurface);
};

}  

#endif  

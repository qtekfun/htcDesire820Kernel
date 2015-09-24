// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_OUTPUT_SOFTWARE_OUTPUT_DEVICE_H_
#define CC_OUTPUT_SOFTWARE_OUTPUT_DEVICE_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "cc/base/cc_export.h"
#include "skia/ext/refptr.h"
#include "third_party/skia/include/core/SkDevice.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"
#include "ui/gfx/vector2d.h"

class SkBitmap;
class SkCanvas;

namespace gfx {
class VSyncProvider;
}

namespace cc {

class SoftwareFrameData;

class CC_EXPORT SoftwareOutputDevice {
 public:
  SoftwareOutputDevice();
  virtual ~SoftwareOutputDevice();

  
  
  
  virtual void Resize(gfx::Size size);

  
  
  
  
  virtual SkCanvas* BeginPaint(gfx::Rect damage_rect);

  
  
  
  virtual void EndPaint(SoftwareFrameData* frame_data);

  
  
  virtual void CopyToBitmap(gfx::Rect rect, SkBitmap* output);

  
  
  virtual void Scroll(gfx::Vector2d delta,
                      gfx::Rect clip_rect);

  
  virtual void DiscardBackbuffer() {}

  
  virtual void EnsureBackbuffer() {}

  
  
  
  
  virtual void ReclaimSoftwareFrame(unsigned id);

  
  
  virtual gfx::VSyncProvider* GetVSyncProvider();

 protected:
  gfx::Size viewport_size_;
  gfx::Rect damage_rect_;
  skia::RefPtr<SkBaseDevice> device_;
  skia::RefPtr<SkCanvas> canvas_;
  scoped_ptr<gfx::VSyncProvider> vsync_provider_;

 private:
  DISALLOW_COPY_AND_ASSIGN(SoftwareOutputDevice);
};

}  

#endif  

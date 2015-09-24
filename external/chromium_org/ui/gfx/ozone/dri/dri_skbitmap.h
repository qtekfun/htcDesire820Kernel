// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_OZONE_DRI_DRI_SKBITMAP_H_
#define UI_GFX_OZONE_DRI_DRI_SKBITMAP_H_

#include "base/basictypes.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/gfx_export.h"

namespace gfx {

class GFX_EXPORT DriSkBitmap : public SkBitmap {
 public:
  DriSkBitmap(int fd);
  virtual ~DriSkBitmap();

  
  
  virtual bool Initialize();

  uint32_t get_handle() const { return handle_; };

  uint32_t get_framebuffer() const { return framebuffer_; };

  int get_fd() const { return fd_; };

  
  uint8_t GetColorDepth() const;

 private:
  friend class DriAllocator;
  friend class HardwareDisplayController;

  void set_handle(uint32_t handle) { handle_ = handle; };
  void set_framebuffer(uint32_t framebuffer) { framebuffer_ = framebuffer; };

  
  
  int fd_;

  
  uint32_t handle_;

  
  
  uint32_t framebuffer_;

  DISALLOW_COPY_AND_ASSIGN(DriSkBitmap);
};

}  

#endif  

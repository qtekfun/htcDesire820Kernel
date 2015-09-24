// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_SURFACE_ACCELERATED_SURFACE_MAC_H_
#define UI_SURFACE_ACCELERATED_SURFACE_MAC_H_

#include <CoreFoundation/CoreFoundation.h>

#include "base/callback.h"
#include "base/mac/scoped_cftyperef.h"
#include "base/memory/scoped_ptr.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"
#include "ui/gl/gl_context.h"
#include "ui/gl/gl_surface.h"
#include "ui/gl/gpu_preference.h"
#include "ui/surface/surface_export.h"

typedef struct _CGLContextObject* CGLContextObj;
typedef unsigned int GLenum;
typedef unsigned int GLuint;

namespace gfx {
class Rect;
}


class SURFACE_EXPORT AcceleratedSurface {
 public:
  AcceleratedSurface();
  virtual ~AcceleratedSurface();

  
  
  
  
  
  
  
  
  
  
  
  
  
  bool Initialize(gfx::GLContext* share_context,
                  bool allocate_fbo,
                  gfx::GpuPreference gpu_preference);
  
  void Destroy();

  

  
  
  
  
  uint32 SetSurfaceSize(const gfx::Size& size);

  
  uint32 GetSurfaceId();

  
  
  bool MakeCurrent();
  
  
  void Clear(const gfx::Rect& rect);
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void SwapBuffers();

  CGLContextObj context() {
    return static_cast<CGLContextObj>(gl_context_->GetHandle());
  }

  
  gfx::Size GetSize() const { return surface_size_; }

 private:
  
  
  
  void AllocateRenderBuffers(GLenum target, const gfx::Size& size);

  
  
  
  
  
  bool SetupFrameBufferObject(GLenum target);

  gfx::Size ClampToValidDimensions(const gfx::Size& size);

  
  
  scoped_refptr<gfx::GLSurface> gl_surface_;
  scoped_refptr<gfx::GLContext> gl_context_;
  base::ScopedCFTypeRef<CFTypeRef> io_surface_;

  
  uint32 io_surface_id_;

  gfx::Size surface_size_;
  
  
  
  
  gfx::Size real_surface_size_;
  
  
  
  
  bool allocate_fbo_;
  
  
  GLuint texture_;
  
  
  GLuint fbo_;
};

#endif  

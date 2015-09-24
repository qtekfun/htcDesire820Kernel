// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_ANDROID_SYNCHRONOUS_COMPOSITOR_H_
#define CONTENT_PUBLIC_BROWSER_ANDROID_SYNCHRONOUS_COMPOSITOR_H_

#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"
#include "gpu/command_buffer/service/in_process_command_buffer.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"

class SkCanvas;

namespace gfx {
class GLSurface;
class Transform;
};

namespace content {

class WebContents;

class SynchronousCompositorClient;

struct CONTENT_EXPORT SynchronousCompositorMemoryPolicy {
  
  size_t bytes_limit;

  
  size_t num_resources_limit;

  SynchronousCompositorMemoryPolicy();

  bool operator==(const SynchronousCompositorMemoryPolicy& other) const;
  bool operator!=(const SynchronousCompositorMemoryPolicy& other) const;
};

class CONTENT_EXPORT SynchronousCompositor {
 public:
  
  
  static void SetClientForWebContents(WebContents* contents,
                                      SynchronousCompositorClient* client);

  
  
  
  
  virtual void SetClient(SynchronousCompositorClient* client) = 0;

  static void SetGpuService(
      scoped_refptr<gpu::InProcessCommandBuffer::Service> service);

  
  
  
  
  
  virtual bool InitializeHwDraw(scoped_refptr<gfx::GLSurface> surface) = 0;

  
  
  
  virtual void ReleaseHwDraw() = 0;

  
  
  
  virtual bool DemandDrawHw(
      gfx::Size surface_size,
      const gfx::Transform& transform,
      gfx::Rect viewport,
      gfx::Rect clip,
      bool stencil_enabled) = 0;

  
  
  virtual bool DemandDrawSw(SkCanvas* canvas) = 0;

  
  virtual void SetMemoryPolicy(
      const SynchronousCompositorMemoryPolicy& policy) = 0;

  
  
  
  virtual void DidChangeRootLayerScrollOffset() = 0;

 protected:
  virtual ~SynchronousCompositor() {}
};

}  

#endif  

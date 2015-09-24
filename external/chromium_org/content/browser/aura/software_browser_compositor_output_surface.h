// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_AURA_SOFTWARE_BROWSER_COMPOSITOR_OUTPUT_SURFACE_H_
#define CONTENT_BROWSER_AURA_SOFTWARE_BROWSER_COMPOSITOR_OUTPUT_SURFACE_H_

#include "base/memory/weak_ptr.h"
#include "content/browser/aura/browser_compositor_output_surface.h"
#include "content/common/content_export.h"
#include "ui/compositor/compositor.h"

namespace base {
class MessageLoopProxy;
}

namespace cc {
class SoftwareOutputDevice;
}

namespace content {

class BrowserCompositorOutputSurfaceProxy;

class CONTENT_EXPORT SoftwareBrowserCompositorOutputSurface
    : public BrowserCompositorOutputSurface {
 public:
  SoftwareBrowserCompositorOutputSurface(
      scoped_refptr<BrowserCompositorOutputSurfaceProxy> surface_proxy,
      scoped_ptr<cc::SoftwareOutputDevice> software_device,
      int surface_id,
      IDMap<BrowserCompositorOutputSurface>* output_surface_map,
      base::MessageLoopProxy* compositor_message_loop,
      base::WeakPtr<ui::Compositor> compositor);

  virtual ~SoftwareBrowserCompositorOutputSurface();

 private:
  virtual void SwapBuffers(cc::CompositorFrame* frame) OVERRIDE;

  
  
  scoped_refptr<BrowserCompositorOutputSurfaceProxy> output_surface_proxy_;

  DISALLOW_COPY_AND_ASSIGN(SoftwareBrowserCompositorOutputSurface);
};

}  

#endif  

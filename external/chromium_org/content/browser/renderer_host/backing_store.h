// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_BACKING_STORE_H_
#define CONTENT_BROWSER_RENDERER_HOST_BACKING_STORE_H_

#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "content/common/content_export.h"
#include "ui/gfx/size.h"
#include "ui/gfx/vector2d.h"
#include "ui/surface/transport_dib.h"

class RenderProcessHost;

namespace gfx {
class Rect;
}

namespace skia {
class PlatformBitmap;
}

namespace content {
class RenderProcessHost;
class RenderWidgetHost;

class CONTENT_EXPORT BackingStore {
 public:
  virtual ~BackingStore();

  RenderWidgetHost* render_widget_host() const {
    return render_widget_host_;
  }
  const gfx::Size& size() { return size_; }

  
  
  
  
  virtual size_t MemorySize();

  
  
  
  
  
  
  
  
  
  virtual void PaintToBackingStore(
      RenderProcessHost* process,
      TransportDIB::Id bitmap,
      const gfx::Rect& bitmap_rect,
      const std::vector<gfx::Rect>& copy_rects,
      float scale_factor,
      const base::Closure& completion_callback,
      bool* scheduled_completion_callback) = 0;

  
  
  
  
  virtual bool CopyFromBackingStore(const gfx::Rect& rect,
                                    skia::PlatformBitmap* output) = 0;

  
  
  virtual void ScrollBackingStore(const gfx::Vector2d& delta,
                                  const gfx::Rect& clip_rect,
                                  const gfx::Size& view_size) = 0;
 protected:
  
  BackingStore(RenderWidgetHost* widget, const gfx::Size& size);

 private:
  
  RenderWidgetHost* render_widget_host_;

  
  gfx::Size size_;

  DISALLOW_COPY_AND_ASSIGN(BackingStore);
};

}  

#endif  

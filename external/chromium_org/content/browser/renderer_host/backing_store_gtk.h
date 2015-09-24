// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_BACKING_STORE_GTK_H_
#define CONTENT_BROWSER_RENDERER_HOST_BACKING_STORE_GTK_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "build/build_config.h"
#include "content/browser/renderer_host/backing_store.h"
#include "content/common/content_export.h"
#include "ui/gfx/x/x11_types.h"

namespace gfx {
class Point;
class Rect;
}

typedef struct _GdkDrawable GdkDrawable;

namespace content {

class CONTENT_EXPORT BackingStoreGtk : public BackingStore {
 public:
  
  
  
  BackingStoreGtk(RenderWidgetHost* widget,
                  const gfx::Size& size,
                  void* visual,
                  int depth);

  
  
  BackingStoreGtk(RenderWidgetHost* widget, const gfx::Size& size);

  virtual ~BackingStoreGtk();

  XDisplay* display() const { return display_; }
  XID root_window() const { return root_window_; }

  
  
  
  
  void XShowRect(const gfx::Point &origin, const gfx::Rect& damage,
                 XID target);

#if defined(TOOLKIT_GTK)
  
  void PaintToRect(const gfx::Rect& dest_rect, GdkDrawable* target);
#endif

  
  virtual size_t MemorySize() OVERRIDE;
  virtual void PaintToBackingStore(
      RenderProcessHost* process,
      TransportDIB::Id bitmap,
      const gfx::Rect& bitmap_rect,
      const std::vector<gfx::Rect>& copy_rects,
      float scale_factor,
      const base::Closure& completion_callback,
      bool* scheduled_completion_callback) OVERRIDE;
  virtual bool CopyFromBackingStore(const gfx::Rect& rect,
                                    skia::PlatformBitmap* output) OVERRIDE;
  virtual void ScrollBackingStore(const gfx::Vector2d& delta,
                                  const gfx::Rect& clip_rect,
                                  const gfx::Size& view_size) OVERRIDE;

 private:
  
  
  void PaintRectWithoutXrender(TransportDIB* bitmap,
                               const gfx::Rect& bitmap_rect,
                               const std::vector<gfx::Rect>& copy_rects);

  
  
  XDisplay* const display_;
  
  const ui::SharedMemorySupport shared_memory_support_;
  
  const bool use_render_;
  
  int pixmap_bpp_;
  
  void* const visual_;
  
  const int visual_depth_;
  
  const XID root_window_;
  
  XID pixmap_;
  
  XID picture_;
  
  void* pixmap_gc_;

  DISALLOW_COPY_AND_ASSIGN(BackingStoreGtk);
};

}  

#endif  

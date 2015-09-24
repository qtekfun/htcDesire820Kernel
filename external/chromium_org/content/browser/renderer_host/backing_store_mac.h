// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_BACKING_STORE_MAC_H_
#define CONTENT_BROWSER_RENDERER_HOST_BACKING_STORE_MAC_H_

#include "base/basictypes.h"
#include "base/mac/scoped_cftyperef.h"
#include "content/browser/renderer_host/backing_store.h"

namespace content {

class BackingStoreMac : public BackingStore {
 public:
  
  
  BackingStoreMac(RenderWidgetHost* widget,
                  const gfx::Size& size,
                  float device_scale_factor);
  virtual ~BackingStoreMac();

  
  
  CGLayerRef cg_layer() { return cg_layer_; }

  
  
  CGContextRef cg_bitmap() { return cg_bitmap_; }

  
  void ScaleFactorChanged(float device_scale_factor);

  
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

  void CopyFromBackingStoreToCGContext(const CGRect& dest_rect,
                                       CGContextRef context);

 private:
  
  
  
  
  CGLayerRef CreateCGLayer();

  
  
  
  CGContextRef CreateCGBitmapContext();

  base::ScopedCFTypeRef<CGContextRef> cg_bitmap_;
  base::ScopedCFTypeRef<CGLayerRef> cg_layer_;

  
  float device_scale_factor_;

  DISALLOW_COPY_AND_ASSIGN(BackingStoreMac);
};

}  

#endif  

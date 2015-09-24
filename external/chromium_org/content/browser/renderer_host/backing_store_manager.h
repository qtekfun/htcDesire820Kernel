// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_BACKING_STORE_MANAGER_H_
#define CONTENT_BROWSER_RENDERER_HOST_BACKING_STORE_MANAGER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/process/process.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"
#include "ui/surface/transport_dib.h"

namespace content {
class BackingStore;
class RenderWidgetHost;

class BackingStoreManager {
 public:
  
  
  
  
  
  static BackingStore* GetBackingStore(RenderWidgetHost* host,
                                       const gfx::Size& desired_size);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static void PrepareBackingStore(
      RenderWidgetHost* host,
      const gfx::Size& backing_store_size,
      TransportDIB::Id bitmap,
      const gfx::Rect& bitmap_rect,
      const std::vector<gfx::Rect>& copy_rects,
      float scale_factor,
      const base::Closure& completion_callback,
      bool* needs_full_paint,
      bool* scheduled_completion_callback);

  
  
  static BackingStore* Lookup(RenderWidgetHost* host);

  
  static void RemoveBackingStore(RenderWidgetHost* host);

  
  static void RemoveAllBackingStores();

  
  static size_t MemorySize();

 private:
  
  BackingStoreManager() {}

  DISALLOW_COPY_AND_ASSIGN(BackingStoreManager);
};

}  

#endif  

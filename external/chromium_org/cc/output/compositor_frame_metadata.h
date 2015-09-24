// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_OUTPUT_COMPOSITOR_FRAME_METADATA_H_
#define CC_OUTPUT_COMPOSITOR_FRAME_METADATA_H_

#include "cc/base/cc_export.h"
#include "ui/events/latency_info.h"
#include "ui/gfx/size_f.h"
#include "ui/gfx/vector2d_f.h"

namespace cc {

class CC_EXPORT CompositorFrameMetadata {
 public:
  CompositorFrameMetadata();
  ~CompositorFrameMetadata();

  
  float device_scale_factor;

  
  
  gfx::Vector2dF root_scroll_offset;
  float page_scale_factor;

  
  
  gfx::SizeF viewport_size;
  gfx::SizeF root_layer_size;
  float min_page_scale_factor;
  float max_page_scale_factor;

  
  
  gfx::Vector2dF location_bar_offset;
  gfx::Vector2dF location_bar_content_translation;
  float overdraw_bottom_height;

  ui::LatencyInfo latency_info;
};

}  

#endif  

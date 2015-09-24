// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_WEBPLUGIN_GEOMETRY_H_
#define CONTENT_COMMON_WEBPLUGIN_GEOMETRY_H_

#include <vector>

#include "base/basictypes.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"

namespace content {

struct WebPluginGeometry {
  WebPluginGeometry();
  ~WebPluginGeometry();

  bool Equals(const WebPluginGeometry& rhs) const;

  
  
  
  
  
  
  
  
  
  
  
  
  gfx::PluginWindowHandle window;
  gfx::Rect window_rect;
  
  
  gfx::Rect clip_rect;
  std::vector<gfx::Rect> cutout_rects;
  bool rects_valid;
  bool visible;
};

}  

#endif  

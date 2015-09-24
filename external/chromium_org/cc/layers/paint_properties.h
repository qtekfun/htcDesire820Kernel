// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_LAYERS_PAINT_PROPERTIES_H_
#define CC_LAYERS_PAINT_PROPERTIES_H_

#include "ui/gfx/size.h"

namespace cc {

struct CC_EXPORT PaintProperties {
  PaintProperties() : source_frame_number(-1) {}

  gfx::Size bounds;

  int source_frame_number;
};

}  

#endif  

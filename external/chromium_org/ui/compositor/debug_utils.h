// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_DEBUG_UTILS_H_
#define UI_COMPOSITOR_DEBUG_UTILS_H_

#include "ui/compositor/compositor_export.h"

namespace gfx {
class Point;
}

namespace ui {

class Layer;

COMPOSITOR_EXPORT void PrintLayerHierarchy(const Layer* layer,
                                           gfx::Point mouse_location);

} 

#endif  

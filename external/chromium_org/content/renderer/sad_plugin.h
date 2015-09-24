// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_SAD_PLUGIN_H_
#define CONTENT_RENDERER_SAD_PLUGIN_H_

#include "third_party/WebKit/public/platform/WebCanvas.h"

class SkBitmap;

namespace gfx {
class Rect;
}

namespace content {

void PaintSadPlugin(blink::WebCanvas* canvas,
                    const gfx::Rect& plugin_rect,
                    const SkBitmap& sad_plugin_bitmap);

}  

#endif  

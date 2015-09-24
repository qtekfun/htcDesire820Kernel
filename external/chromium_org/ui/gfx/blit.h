// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_BLIT_H_
#define UI_GFX_BLIT_H_

#include "ui/gfx/gfx_export.h"
#include "ui/gfx/native_widget_types.h"

class SkCanvas;

namespace gfx {

class Point;
class Rect;
class Vector2d;

GFX_EXPORT void BlitContextToContext(NativeDrawingContext dst_context,
                                     const Rect& dst_rect,
                                     NativeDrawingContext src_context,
                                     const Point& src_origin);

GFX_EXPORT void BlitContextToCanvas(SkCanvas *dst_canvas,
                                    const Rect& dst_rect,
                                    NativeDrawingContext src_context,
                                    const Point& src_origin);

GFX_EXPORT void BlitCanvasToContext(NativeDrawingContext dst_context,
                                    const Rect& dst_rect,
                                    SkCanvas *src_canvas,
                                    const Point& src_origin);

GFX_EXPORT void BlitCanvasToCanvas(SkCanvas *dst_canvas,
                                   const Rect& dst_rect,
                                   SkCanvas *src_canvas,
                                   const Point& src_origin);

GFX_EXPORT void ScrollCanvas(SkCanvas* canvas,
                             const Rect& clip,
                             const Vector2d& offset);

}  

#endif  

// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_CURSOR_CURSORS_AURA_H_
#define UI_BASE_CURSOR_CURSORS_AURA_H_

#include "ui/base/cursor/cursor.h"
#include "ui/base/ui_export.h"

class SkBitmap;

namespace gfx {
class Point;
}

namespace ui {

const int kAnimatedCursorFrameDelayMs = 25;

bool UI_EXPORT GetCursorDataFor(CursorSetType cursor_set_id,
                                int id,
                                float scale_factor,
                                int* resource_id,
                                gfx::Point* point);

bool UI_EXPORT GetAnimatedCursorDataFor(CursorSetType cursor_set_id,
                                        int id,
                                        float scale_factor,
                                        int* resource_id,
                                        gfx::Point* point);

bool UI_EXPORT GetCursorBitmap(const Cursor& cursor,
                               SkBitmap* bitmap,
                               gfx::Point* point);

}  

#endif  

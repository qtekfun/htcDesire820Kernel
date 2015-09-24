// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_SNAPSHOT_SNAPSHOT_WIN_H_
#define UI_SNAPSHOT_SNAPSHOT_WIN_H_

#include <windows.h>

#include <vector>

#include "ui/snapshot/snapshot_export.h"

namespace gfx {
class Rect;
}

namespace ui {
namespace internal {

SNAPSHOT_EXPORT bool GrabHwndSnapshot(
    HWND window_handle,
    const gfx::Rect& snapshot_bounds,
    std::vector<unsigned char>* png_representation);

}  
}  

#endif  

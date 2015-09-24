// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_SNAPSHOT_TEST_SNAPSHOT_DESKTOP_H_
#define UI_SNAPSHOT_TEST_SNAPSHOT_DESKTOP_H_

#include <vector>

namespace gfx {
class Rect;
}

namespace ui {

bool GrabDesktopSnapshot(
    const gfx::Rect& snapshot_bounds,
    std::vector<unsigned char>* png_representation);

}  

#endif  

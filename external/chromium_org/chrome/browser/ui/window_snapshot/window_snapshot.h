// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WINDOW_SNAPSHOT_WINDOW_SNAPSHOT_H_
#define CHROME_BROWSER_UI_WINDOW_SNAPSHOT_WINDOW_SNAPSHOT_H_

#include <vector>

#include "ui/gfx/native_widget_types.h"

class PrefRegistrySimple;

namespace gfx {
class Rect;
}

namespace chrome {

void RegisterScreenshotPrefs(PrefRegistrySimple* registry);

bool GrabWindowSnapshotForUser(
    gfx::NativeWindow window,
    std::vector<unsigned char>* png_representation,
    const gfx::Rect& snapshot_bounds);

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_ASH_ASH_UTIL_H_
#define CHROME_BROWSER_UI_ASH_ASH_UTIL_H_

#include "ui/gfx/native_widget_types.h"

namespace chrome {

bool IsNativeViewInAsh(gfx::NativeView native_view);
bool IsNativeWindowInAsh(gfx::NativeWindow native_window);

void ToggleAshDesktop();

}  

#endif  

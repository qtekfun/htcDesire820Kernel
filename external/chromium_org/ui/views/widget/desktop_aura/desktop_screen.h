// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_SCREEN_H_
#define UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_SCREEN_H_

#include "ui/views/views_export.h"

namespace gfx {
class Screen;
}

namespace views {

VIEWS_EXPORT gfx::Screen* CreateDesktopScreen();

}  

#endif  

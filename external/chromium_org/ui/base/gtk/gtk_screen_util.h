// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_GTK_GTK_SCREEN_UTILS_H_
#define UI_BASE_GTK_GTK_SCREEN_UTILS_H_

#include <gtk/gtk.h>

#include "ui/base/ui_export.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/vector2d.h"

namespace ui {

UI_EXPORT bool IsScreenComposited();

UI_EXPORT gfx::Point ScreenPoint(GtkWidget* widget);

UI_EXPORT gfx::Point ClientPoint(GtkWidget* widget);

UI_EXPORT gfx::Vector2d GetWidgetScreenOffset(GtkWidget* widget);

UI_EXPORT gfx::Rect GetWidgetScreenBounds(GtkWidget* widget);

}  

#endif  

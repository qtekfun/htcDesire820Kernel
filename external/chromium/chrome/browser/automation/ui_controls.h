// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOMATION_UI_CONTROLS_H_
#define CHROME_BROWSER_AUTOMATION_UI_CONTROLS_H_
#pragma once

#include "build/build_config.h"

#if defined(OS_WIN)
#include <wtypes.h>
#endif

#include "ui/base/keycodes/keyboard_codes.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"

#if defined(TOOLKIT_VIEWS)
namespace views {
class View;
}
#endif

class Task;

namespace ui_controls {


bool SendKeyPress(gfx::NativeWindow window,
                  ui::KeyboardCode key,
                  bool control,
                  bool shift,
                  bool alt,
                  bool command);
bool SendKeyPressNotifyWhenDone(gfx::NativeWindow window,
                                ui::KeyboardCode key,
                                bool control,
                                bool shift,
                                bool alt,
                                bool command,
                                Task* task);

bool SendMouseMove(long x, long y);
bool SendMouseMoveNotifyWhenDone(long x, long y, Task* task);

enum MouseButton {
  LEFT = 0,
  MIDDLE,
  RIGHT,
};

enum MouseButtonState {
  UP = 1,
  DOWN = 2
};

bool SendMouseEvents(MouseButton type, int state);
bool SendMouseEventsNotifyWhenDone(MouseButton type, int state, Task* task);
bool SendMouseClick(MouseButton type);

void MoveMouseToCenterAndPress(
#if defined(TOOLKIT_VIEWS)
    views::View* view,
#elif defined(TOOLKIT_GTK)
    GtkWidget* widget,
#elif defined(OS_MACOSX)
    NSView* view,
#endif
    MouseButton button,
    int state,
    Task* task);

}  

#endif  

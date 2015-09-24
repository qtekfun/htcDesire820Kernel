// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_TEST_UI_CONTROLS_H_
#define UI_BASE_TEST_UI_CONTROLS_H_

#include "base/callback_forward.h"
#include "build/build_config.h"
#include "ui/events/keycodes/keyboard_codes.h"
#include "ui/gfx/native_widget_types.h"

namespace ui_controls {



void EnableUIControls();

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
                                const base::Closure& task);

bool SendMouseMove(long x, long y);
bool SendMouseMoveNotifyWhenDone(long x,
                                 long y,
                                 const base::Closure& task);

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
bool SendMouseEventsNotifyWhenDone(MouseButton type,
                                   int state,
                                   const base::Closure& task);

bool SendMouseClick(MouseButton type);

#if defined(TOOLKIT_VIEWS)
void RunClosureAfterAllPendingUIEvents(const base::Closure& closure);
#endif

#if defined(USE_AURA)
class UIControlsAura;
void InstallUIControlsAura(UIControlsAura* instance);
#endif

}  

#endif  

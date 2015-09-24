// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_KEYBOARD_KEYBOARD_UTIL_H_
#define UI_KEYBOARD_KEYBOARD_UTIL_H_

#include <string>

#include "base/strings/string16.h"
#include "ui/aura/window.h"
#include "ui/keyboard/keyboard_export.h"

struct GritResourceMap;

namespace keyboard {

enum CursorMoveDirection {
  kCursorMoveRight = 0x01,
  kCursorMoveLeft = 0x02,
  kCursorMoveUp = 0x04,
  kCursorMoveDown = 0x08
};

enum KeyboardControlEvent {
  KEYBOARD_CONTROL_SHOW = 0,
  KEYBOARD_CONTROL_HIDE_AUTO,
  KEYBOARD_CONTROL_HIDE_USER,
  KEYBOARD_CONTROL_MAX,
};

KEYBOARD_EXPORT bool IsKeyboardEnabled();

KEYBOARD_EXPORT bool IsKeyboardUsabilityExperimentEnabled();

KEYBOARD_EXPORT bool InsertText(const base::string16& text,
                                aura::Window* root_window);

KEYBOARD_EXPORT bool MoveCursor(int swipe_direction,
                                int modifier_flags,
                                aura::WindowEventDispatcher* dispatcher);

KEYBOARD_EXPORT bool SendKeyEvent(std::string type,
                                   int key_value,
                                   int key_code,
                                   std::string key_name,
                                   int modifiers,
                                   aura::WindowEventDispatcher* dispatcher);

KEYBOARD_EXPORT const void MarkKeyboardLoadStarted();

KEYBOARD_EXPORT const void MarkKeyboardLoadFinished();

KEYBOARD_EXPORT const GritResourceMap* GetKeyboardExtensionResources(
    size_t* size);

void LogKeyboardControlEvent(KeyboardControlEvent event);

}  

#endif  

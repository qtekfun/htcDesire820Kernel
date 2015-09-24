// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_CHROMEDRIVER_CHROME_UI_EVENTS_H_
#define CHROME_TEST_CHROMEDRIVER_CHROME_UI_EVENTS_H_

#include <string>

#include "ui/events/keycodes/keyboard_codes.h"

enum MouseEventType {
  kPressedMouseEventType = 0,
  kReleasedMouseEventType,
  kMovedMouseEventType
};

enum MouseButton {
  kLeftMouseButton = 0,
  kMiddleMouseButton,
  kRightMouseButton,
  kNoneMouseButton
};

struct MouseEvent {
  MouseEvent(MouseEventType type,
             MouseButton button,
             int x,
             int y,
             int modifiers,
             int click_count);
  ~MouseEvent();

  MouseEventType type;
  MouseButton button;
  int x;
  int y;
  int modifiers;
  
  int click_count;
};

enum TouchEventType {
  kTouchStart = 0,
  kTouchEnd,
  kTouchMove,
};

struct TouchEvent {
  TouchEvent(TouchEventType type,
             int x,
             int y);
  ~TouchEvent();

  TouchEventType type;
  int x;
  int y;
};

enum KeyEventType {
  kKeyDownEventType = 0,
  kKeyUpEventType,
  kRawKeyDownEventType,
  kCharEventType
};

enum KeyModifierMask {
  kAltKeyModifierMask = 1 << 0,
  kControlKeyModifierMask = 1 << 1,
  kMetaKeyModifierMask = 1 << 2,
  kShiftKeyModifierMask = 1 << 3,
  kNumLockKeyModifierMask = 1 << 4
};

struct KeyEvent {
  KeyEvent(KeyEventType type,
           int modifiers,
           const std::string& modified_text,
           const std::string& unmodified_text,
           ui::KeyboardCode key_code);
  ~KeyEvent();

  KeyEventType type;
  int modifiers;
  std::string modified_text;
  std::string unmodified_text;
  ui::KeyboardCode key_code;
};

#endif  

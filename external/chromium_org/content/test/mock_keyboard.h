// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_MOCK_KEYBOARD_H_
#define CONTENT_TEST_MOCK_KEYBOARD_H_

#include <string>

#include "base/basictypes.h"

#if defined(OS_WIN)
#include "content/test/mock_keyboard_driver_win.h"
#endif

namespace content {

class MockKeyboard {
 public:
  
  enum Layout {
    LAYOUT_NULL,
    LAYOUT_ARABIC,
    LAYOUT_BULGARIAN,
    LAYOUT_CHINESE_TRADITIONAL,
    LAYOUT_CZECH,
    LAYOUT_DANISH,
    LAYOUT_GERMAN,
    LAYOUT_GREEK,
    LAYOUT_UNITED_STATES,
    LAYOUT_SPANISH,
    LAYOUT_FINNISH,
    LAYOUT_FRENCH,
    LAYOUT_HEBREW,
    LAYOUT_HUNGARIAN,
    LAYOUT_ICELANDIC,
    LAYOUT_ITALIAN,
    LAYOUT_JAPANESE,
    LAYOUT_KOREAN,
    LAYOUT_POLISH,
    LAYOUT_PORTUGUESE_BRAZILIAN,
    LAYOUT_ROMANIAN,
    LAYOUT_RUSSIAN,
    LAYOUT_CROATIAN,
    LAYOUT_SLOVAK,
    LAYOUT_THAI,
    LAYOUT_SWEDISH,
    LAYOUT_TURKISH_Q,
    LAYOUT_VIETNAMESE,
    LAYOUT_DEVANAGARI_INSCRIPT,
    LAYOUT_PORTUGUESE,
    LAYOUT_UNITED_STATES_DVORAK,
    LAYOUT_CANADIAN_FRENCH,
  };

  
  
  
  
  enum Modifiers {
    INVALID = -1,
    NONE = 0,
    LEFT_SHIFT = 1 << 0,
    LEFT_CONTROL = 1 << 1,
    LEFT_ALT = 1 << 2,
    LEFT_META = 1 << 3,
    RIGHT_SHIFT = 1 << 4,
    RIGHT_CONTROL = 1 << 5,
    RIGHT_ALT = 1 << 6,
    RIGHT_META = 1 << 7,
    KEYPAD = 1 << 8,
    AUTOREPEAAT = 1 << 9,
  };

  MockKeyboard();
  ~MockKeyboard();

  
  
  
  
  
  int GetCharacters(Layout layout,
                    int key_code,
                    Modifiers modifiers,
                    std::wstring* output);

 private:
  Layout keyboard_layout_;
  Modifiers keyboard_modifiers_;

#if defined(OS_WIN)
  MockKeyboardDriverWin driver_;
#endif

  DISALLOW_COPY_AND_ASSIGN(MockKeyboard);
};

}  

#endif  

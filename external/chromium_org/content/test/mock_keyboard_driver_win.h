// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_MOCK_KEYBOARD_DRIVER_WIN_H_
#define CONTENT_TEST_MOCK_KEYBOARD_DRIVER_WIN_H_

#include <windows.h>

#include <string>
#include <vector>
#include "base/basictypes.h"

namespace content {

class MockKeyboardDriverWin {
 public:
  MockKeyboardDriverWin();
  ~MockKeyboardDriverWin();

  bool SetLayout(int layout);
  bool SetModifiers(int modifiers);
  int GetCharacters(int key_code, std::wstring* code);

 private:
  void MaybeUnloadActiveLayout();

  
  std::vector<HKL> orig_keyboard_layouts_list_;
  
  HKL original_keyboard_layout_;
  
  HKL active_keyboard_layout_;
  BYTE original_keyboard_states_[256];

  BYTE keyboard_states_[256];

  DISALLOW_COPY_AND_ASSIGN(MockKeyboardDriverWin);
};

}  

#endif  

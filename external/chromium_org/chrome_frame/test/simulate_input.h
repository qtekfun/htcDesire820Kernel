// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_TEST_SIMULATE_INPUT_H_
#define CHROME_FRAME_TEST_SIMULATE_INPUT_H_

#include <windows.h>
#include <string>

#include "base/basictypes.h"
#include "base/process/process_handle.h"

namespace simulate_input {

enum Modifier {
  NONE,
  SHIFT = 1,
  CONTROL = 2,
  ALT = 4
};

enum KeyMode {
  KEY_DOWN,
  KEY_UP,
};

bool ForceSetForegroundWindow(HWND window);

bool EnsureProcessInForeground(base::ProcessId process_id);

void SetKeyboardFocusToWindow(HWND window);

void SendMnemonic(WORD mnemonic_char, uint32 modifiers, bool extended,
                  bool unicode, KeyMode key_mode);

enum MouseButton { LEFT, RIGHT, MIDDLE, X };
void SendMouseClick(int x, int y, MouseButton button);
void SendMouseClick(HWND window, int x, int y, MouseButton button);

void SendScanCode(short scan_code, uint32 modifiers);
void SendCharA(char c, uint32 modifiers);
void SendCharW(wchar_t c, uint32 modifiers);

void SendExtendedKey(WORD key, uint32 modifiers);

void SendStringW(const std::wstring& s);
void SendStringA(const std::string& s);

}  

#endif  


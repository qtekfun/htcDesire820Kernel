// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_GLOBAL_KEYBOARD_SHORTCUTS_MAC_H_
#define CHROME_BROWSER_GLOBAL_KEYBOARD_SHORTCUTS_MAC_H_

#include "base/basictypes.h"

struct KeyboardShortcutData {
  bool command_key;
  bool shift_key;
  bool cntrl_key;
  bool opt_key;
  
  
  
  
  int vkey_code;  
  unichar key_char;  
                     
  int chrome_command;  
};


int CommandForWindowKeyboardShortcut(
    bool command_key, bool shift_key, bool cntrl_key, bool opt_key,
    int vkey_code, unichar key_char);

int CommandForDelayedWindowKeyboardShortcut(
    bool command_key, bool shift_key, bool cntrl_key, bool opt_key,
    int vkey_code, unichar key_char);

int CommandForBrowserKeyboardShortcut(
    bool command_key, bool shift_key, bool cntrl_key, bool opt_key,
    int vkey_code, unichar key_char);

unichar KeyCharacterForEvent(NSEvent* event);

const KeyboardShortcutData* GetWindowKeyboardShortcutTable(size_t* num_entries);
const KeyboardShortcutData*
    GetDelayedWindowKeyboardShortcutTable(size_t* num_entries);
const KeyboardShortcutData*
    GetBrowserKeyboardShortcutTable(size_t* num_entries);

#endif  

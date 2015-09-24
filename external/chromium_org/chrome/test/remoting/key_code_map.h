// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_REMOTING_KEY_CODE_MAP_H_
#define CHROME_TEST_REMOTING_KEY_CODE_MAP_H_

#include "base/basictypes.h"
#include "ui/events/keycodes/keyboard_codes.h"

namespace remoting {

typedef struct {
  
  char lower_char;

  
  char upper_char;

  
  
  const char* code;

  
  ui::KeyboardCode vkey_code;
} KeyCodeMap;

const KeyCodeMap key_code_map[] = {

  {'a', 'A', "KeyA", ui::VKEY_A},  
  {'b', 'B', "KeyB", ui::VKEY_B},  
  {'c', 'C', "KeyC", ui::VKEY_C},  
  {'d', 'D', "KeyD", ui::VKEY_D},  
  {'e', 'E', "KeyE", ui::VKEY_E},  
  {'f', 'F', "KeyF", ui::VKEY_F},  
  {'g', 'G', "KeyG", ui::VKEY_G},  
  {'h', 'H', "KeyH", ui::VKEY_H},  
  {'i', 'I', "KeyI", ui::VKEY_I},  
  {'j', 'J', "KeyJ", ui::VKEY_J},  
  {'k', 'K', "KeyK", ui::VKEY_K},  
  {'l', 'L', "KeyL", ui::VKEY_L},  
  {'m', 'M', "KeyM", ui::VKEY_M},  
  {'n', 'N', "KeyN", ui::VKEY_N},  
  {'o', 'O', "KeyO", ui::VKEY_O},  
  {'p', 'P', "KeyP", ui::VKEY_P},  
  {'q', 'Q', "KeyQ", ui::VKEY_Q},  
  {'r', 'R', "KeyR", ui::VKEY_R},  
  {'s', 'S', "KeyS", ui::VKEY_S},  
  {'t', 'T', "KeyT", ui::VKEY_T},  
  {'u', 'U', "KeyU", ui::VKEY_U},  
  {'v', 'V', "KeyV", ui::VKEY_V},  
  {'w', 'W', "KeyW", ui::VKEY_W},  
  {'x', 'X', "KeyX", ui::VKEY_X},  
  {'y', 'Y', "KeyY", ui::VKEY_Y},  
  {'z', 'Z', "KeyZ", ui::VKEY_Z},  
  {'1', '1', "Digit1", ui::VKEY_0},  
  {'2', '2', "Digit2", ui::VKEY_1},  
  {'3', '3', "Digit3", ui::VKEY_2},  
  {'4', '4', "Digit4", ui::VKEY_3},  
  {'5', '5', "Digit5", ui::VKEY_4},  
  {'6', '6', "Digit6", ui::VKEY_5},  
  {'7', '7', "Digit7", ui::VKEY_6},  
  {'8', '8', "Digit8", ui::VKEY_7},  
  {'9', '9', "Digit9", ui::VKEY_8},  
  {'0', '0', "Digit0", ui::VKEY_9},  

  {'\n', '\n', "Enter", ui::VKEY_RETURN},  
  { 0 ,  0 , "Escape", ui::VKEY_UNKNOWN},  
  {'\b', '\b', "Backspace", ui::VKEY_BACK},  
  {'\t', '\t', "Tab", ui::VKEY_TAB},  
  {' ', ' ', "Space", ui::VKEY_SPACE},  
  {'-', '_', "Minus", ui::VKEY_OEM_MINUS},  
  {'=', '+', "Equal", ui::VKEY_OEM_PLUS},  
  {'[', '{', "BracketLeft", ui::VKEY_OEM_4},  
  {']', '}', "BracketRight", ui::VKEY_OEM_6},  
  {'\\', '|', "Backslash", ui::VKEY_OEM_5},  
  {';', ':', "Semicolon", ui::VKEY_OEM_1},  
  {'\'', '\"', "Quote", ui::VKEY_OEM_7},  
  {'`', '~', "Backquote", ui::VKEY_OEM_3},  
  {',', '<', "Comma", ui::VKEY_OEM_COMMA},  
  {'.', '>', "Period", ui::VKEY_OEM_PERIOD},  
  {'/', '?', "Slash", ui::VKEY_OEM_2},  
};

}  

#endif  

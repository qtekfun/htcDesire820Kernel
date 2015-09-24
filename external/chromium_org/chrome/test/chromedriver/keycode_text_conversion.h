// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_CHROMEDRIVER_KEYCODE_TEXT_CONVERSION_H_
#define CHROME_TEST_CHROMEDRIVER_KEYCODE_TEXT_CONVERSION_H_

#include <string>

#include "base/strings/string16.h"
#include "ui/events/keycodes/keyboard_codes.h"


bool ConvertKeyCodeToText(ui::KeyboardCode key_code,
                          int modifiers,
                          std::string* text,
                          std::string* error_msg);

bool ConvertCharToKeyCode(char16 key,
                          ui::KeyboardCode* key_code,
                          int *necessary_modifiers,
                          std::string* error_msg);

#endif  

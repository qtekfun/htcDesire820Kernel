// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_CHROMEDRIVER_KEY_CONVERTER_H_
#define CHROME_TEST_CHROMEDRIVER_KEY_CONVERTER_H_

#include <list>
#include <string>

#include "base/strings/string16.h"
#include "ui/events/keycodes/keyboard_codes.h"

struct KeyEvent;
class Status;

KeyEvent CreateKeyDownEvent(ui::KeyboardCode key_code, int modifiers);
KeyEvent CreateKeyUpEvent(ui::KeyboardCode key_code, int modifiers);
KeyEvent CreateCharEvent(const std::string& unmodified_text,
                         const std::string& modified_text,
                         int modifiers);

Status ConvertKeysToKeyEvents(const string16& keys,
                              bool release_modifiers,
                              int* modifiers,
                              std::list<KeyEvent>* key_events);

#endif  

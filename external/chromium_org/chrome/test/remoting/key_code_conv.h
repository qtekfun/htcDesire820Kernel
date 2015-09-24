// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_REMOTING_KEY_CODE_CONV_H_
#define CHROME_TEST_REMOTING_KEY_CODE_CONV_H_

#include "ui/events/keycodes/keyboard_codes.h"

namespace remoting {

void GetKeyValuesFromChar(
    char c, const char** code, ui::KeyboardCode* vkey_code, bool* shift);

}  

#endif  

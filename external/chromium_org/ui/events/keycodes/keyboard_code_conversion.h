// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_EVENTS_KEYCODES_KEYBOARD_CODE_CONVERSION_H_
#define UI_EVENTS_KEYCODES_KEYBOARD_CODE_CONVERSION_H_

#include "base/basictypes.h"
#include "ui/events/events_base_export.h"
#include "ui/events/keycodes/keyboard_codes.h"

namespace ui {

EVENTS_BASE_EXPORT uint16 GetCharacterFromKeyCode(KeyboardCode key_code,
                                                  int flags);

}  

#endif  

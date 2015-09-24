// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_EVENTS_KEYCODES_KEYBOARD_CODE_CONVERSION_MAC_H_
#define UI_EVENTS_KEYCODES_KEYBOARD_CODE_CONVERSION_MAC_H_

#import <Cocoa/Cocoa.h>

#include "base/basictypes.h"
#include "ui/events/events_base_export.h"
#include "ui/events/keycodes/keyboard_codes_posix.h"

namespace ui {

EVENTS_BASE_EXPORT int MacKeyCodeForWindowsKeyCode(
    KeyboardCode keycode,
    NSUInteger flags,
    unichar* character,
    unichar* characterIgnoringModifiers);

EVENTS_BASE_EXPORT KeyboardCode KeyboardCodeFromNSEvent(NSEvent* event);

EVENTS_BASE_EXPORT const char* CodeFromNSEvent(NSEvent* event);

} 

#endif  

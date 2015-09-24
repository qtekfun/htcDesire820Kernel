// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GAMEPAD_GAMEPAD_STANDARD_MAPPINGS_H_
#define CONTENT_BROWSER_GAMEPAD_GAMEPAD_STANDARD_MAPPINGS_H_

#include "base/strings/string_piece.h"

namespace blink {
class WebGamepad;
}

namespace content {

typedef void (*GamepadStandardMappingFunction)(
    const blink::WebGamepad& original,
    blink::WebGamepad* mapped);

GamepadStandardMappingFunction GetGamepadStandardMappingFunction(
    const base::StringPiece& vendor_id,
    const base::StringPiece& product_id);


enum CanonicalButtonIndex {
  kButtonPrimary,
  kButtonSecondary,
  kButtonTertiary,
  kButtonQuaternary,
  kButtonLeftShoulder,
  kButtonRightShoulder,
  kButtonLeftTrigger,
  kButtonRightTrigger,
  kButtonBackSelect,
  kButtonStart,
  kButtonLeftThumbstick,
  kButtonRightThumbstick,
  kButtonDpadUp,
  kButtonDpadDown,
  kButtonDpadLeft,
  kButtonDpadRight,
  kButtonMeta,
  kNumButtons
};

enum CanonicalAxisIndex {
  kAxisLeftStickX,
  kAxisLeftStickY,
  kAxisRightStickX,
  kAxisRightStickY,
  kNumAxes
};

}  

#endif  

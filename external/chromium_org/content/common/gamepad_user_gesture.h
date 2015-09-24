// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GAMEPAD_USER_GESTURE_H_
#define CONTENT_COMMON_GAMEPAD_USER_GESTURE_H_

namespace blink {
class WebGamepads;
}

namespace content {

bool GamepadsHaveUserGesture(const blink::WebGamepads& gamepads);

}  

#endif  

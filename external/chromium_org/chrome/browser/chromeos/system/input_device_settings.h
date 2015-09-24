// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_SYSTEM_INPUT_DEVICE_SETTINGS_H_
#define CHROME_BROWSER_CHROMEOS_SYSTEM_INPUT_DEVICE_SETTINGS_H_

#include "base/callback.h"

namespace chromeos {
namespace system {

const int kMinPointerSensitivity = 1;
const int kMaxPointerSensitivity = 5;

typedef base::Callback<void(bool)> DeviceExistsCallback;

namespace touchpad_settings {

void TouchpadExists(const DeviceExistsCallback& callback);

void SetSensitivity(int value);

void SetTapToClick(bool enabled);

void SetThreeFingerClick(bool enabled);

void SetTapDragging(bool enabled);

}  

namespace mouse_settings {

void MouseExists(const DeviceExistsCallback& callback);

void SetSensitivity(int value);

void SetPrimaryButtonRight(bool right);

}  

namespace keyboard_settings {

bool ForceKeyboardDrivenUINavigation();

}  

}  
}  

#endif  

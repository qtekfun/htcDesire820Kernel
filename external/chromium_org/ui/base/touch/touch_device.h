// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_TOUCH_TOUCH_DEVICE_H_
#define UI_BASE_TOUCH_TOUCH_DEVICE_H_

#include "ui/base/ui_export.h"

namespace ui {

const int kMaxTouchPointsUnknown = -1;

UI_EXPORT bool IsTouchDevicePresent();

UI_EXPORT int MaxTouchPoints();

}  

#endif  

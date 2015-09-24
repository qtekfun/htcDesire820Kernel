// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_ANDROID_VIEW_CONFIGURATION_H_
#define UI_GFX_ANDROID_VIEW_CONFIGURATION_H_

#include <jni.h>

#include "ui/gfx/gfx_export.h"

namespace gfx {

class GFX_EXPORT ViewConfiguration {
 public:
  static int GetDoubleTapTimeoutInMs();
  static int GetLongPressTimeoutInMs();
  static int GetTapTimeoutInMs();

  static int GetMaximumFlingVelocityInPixelsPerSecond();
  static int GetMinimumFlingVelocityInPixelsPerSecond();

  static int GetTouchSlopInPixels();

  
  static bool RegisterViewConfiguration(JNIEnv* env);
};

}  

#endif  

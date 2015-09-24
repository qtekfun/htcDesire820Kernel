// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SCREENSHOT_DELEGATE_H_
#define ASH_SCREENSHOT_DELEGATE_H_

namespace aura {
class Window;
}  

namespace gfx {
class Rect;
}  

namespace ash {

class ScreenshotDelegate {
 public:
  virtual ~ScreenshotDelegate() {}

  
  
  virtual void HandleTakeScreenshotForAllRootWindows() = 0;

  
  
  virtual void HandleTakePartialScreenshot(
      aura::Window* window, const gfx::Rect& rect) = 0;

  
  virtual bool CanTakeScreenshot() = 0;
};
}  

#endif  

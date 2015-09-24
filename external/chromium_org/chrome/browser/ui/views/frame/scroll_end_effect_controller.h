 // Copyright 2013 The Chromium Authors. All rights reserved.
 // Use of this source code is governed by a BSD-style license that can be
 // found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_SCROLL_END_EFFECT_CONTROLLER_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_SCROLL_END_EFFECT_CONTROLLER_H_

#include "base/basictypes.h"

class ScrollEndEffectController {
 public:
  ScrollEndEffectController() {}
  virtual ~ScrollEndEffectController() {}

  static ScrollEndEffectController* Create();

  
  
  virtual void OverscrollUpdate(int delta_y) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(ScrollEndEffectController);
};

#endif  

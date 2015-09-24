// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_USER_INPUT_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_USER_INPUT_H_

#include "ui/gfx/rect.h"

namespace chromeos {

class UserInput {
 public:
  virtual ~UserInput() {}

  
  virtual void EnableInputControls(bool enabled) = 0;

  
  virtual void ClearAndFocusControls() = 0;

  
  virtual void ClearAndFocusPassword() = 0;

  
  
  virtual gfx::Rect GetMainInputScreenBounds() const = 0;
};

}  

#endif  

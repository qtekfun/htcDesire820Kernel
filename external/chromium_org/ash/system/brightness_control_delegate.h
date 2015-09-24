// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_BRIGHTNESS_CONTROL_DELEGATE_H_
#define ASH_SYSTEM_BRIGHTNESS_CONTROL_DELEGATE_H_

#include "base/callback.h"

namespace ui {
class Accelerator;
}  

namespace ash {

class BrightnessControlDelegate {
 public:
  virtual ~BrightnessControlDelegate() {}

  
  
  virtual bool HandleBrightnessDown(const ui::Accelerator& accelerator) = 0;
  virtual bool HandleBrightnessUp(const ui::Accelerator& accelerator) = 0;

  
  
  
  virtual void SetBrightnessPercent(double percent, bool gradual) = 0;

  
  
  virtual void GetBrightnessPercent(
      const base::Callback<void(double)>& callback) = 0;
};

}  

#endif  

// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_BRIGHTNESS_OBSERVER_H_
#define CHROME_BROWSER_CHROMEOS_BRIGHTNESS_OBSERVER_H_
#pragma once

#include "base/basictypes.h"
#include "chrome/browser/chromeos/cros/brightness_library.h"

namespace chromeos {

class BrightnessObserver : public BrightnessLibrary::Observer {
 public:
  BrightnessObserver() {}
  virtual ~BrightnessObserver() {}

 private:
  
  virtual void BrightnessChanged(int level, bool user_initiated);

  DISALLOW_COPY_AND_ASSIGN(BrightnessObserver);
};

}  

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_AUTOCLICK_AUTOCLICK_CONTROLLER_H
#define ASH_AUTOCLICK_AUTOCLICK_CONTROLLER_H

#include "ash/ash_export.h"

namespace ash {

class ASH_EXPORT AutoclickController {
 public:
  virtual ~AutoclickController() {}

  
  virtual void SetEnabled(bool enabled) = 0;

  
  virtual bool IsEnabled() const = 0;

  
  
  virtual void SetAutoclickDelay(int delay_ms) = 0;

  
  virtual int GetAutoclickDelay() const = 0;

  static AutoclickController* CreateInstance();

  
  
  static const int kDefaultAutoclickDelayMs;

 protected:
  AutoclickController() {}
};

}  

#endif  

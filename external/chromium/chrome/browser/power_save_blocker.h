// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POWER_SAVE_BLOCKER_H_
#define CHROME_BROWSER_POWER_SAVE_BLOCKER_H_
#pragma once

#include "base/basictypes.h"

class PowerSaveBlocker {
 public:
  explicit PowerSaveBlocker(bool enabled);
  ~PowerSaveBlocker();

  bool enabled() const { return enabled_; }

  
  void Enable();
  
  void Disable();

 private:
  
  
  static void ApplyBlock(bool blocked);

  
  static void AdjustBlockCount(int delta);

  
  static void PostAdjustBlockCount(int delta);

  bool enabled_;

  static int blocker_count_;

  DISALLOW_COPY_AND_ASSIGN(PowerSaveBlocker);
};

#endif  

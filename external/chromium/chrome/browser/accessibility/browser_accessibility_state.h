// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_STATE_H_
#define CHROME_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_STATE_H_
#pragma once

#include "base/basictypes.h"

template <typename T> struct DefaultSingletonTraits;

class BrowserAccessibilityState {
 public:
  
  static BrowserAccessibilityState* GetInstance();

  ~BrowserAccessibilityState();

  
  void OnScreenReaderDetected();

  
  bool IsAccessibleBrowser();

 private:
  BrowserAccessibilityState();
  friend struct DefaultSingletonTraits<BrowserAccessibilityState>;

  
  bool screen_reader_active_;

  DISALLOW_COPY_AND_ASSIGN(BrowserAccessibilityState);
};

#endif  

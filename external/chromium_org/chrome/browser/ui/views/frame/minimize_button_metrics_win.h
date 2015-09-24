// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_MINIMIZE_BUTTON_METRICS_WIN_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_MINIMIZE_BUTTON_METRICS_WIN_H_

#include <windows.h>

#include "base/basictypes.h"

class MinimizeButtonMetrics {
 public:
  MinimizeButtonMetrics();
  ~MinimizeButtonMetrics();

  void Init(HWND hwnd);

  
  
  
  int GetMinimizeButtonOffsetX() const;

  
  
  void OnHWNDActivated();

 private:
  
  int GetAndCacheMinimizeButtonOffsetX() const;

  HWND hwnd_;

  
  
  
  mutable int cached_minimize_button_x_delta_;

  
  static int last_cached_minimize_button_x_delta_;

  
  bool was_activated_;

  DISALLOW_COPY_AND_ASSIGN(MinimizeButtonMetrics);
};

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIN_SCOPED_FULLSCREEN_VISIBILITY_H_
#define UI_VIEWS_WIN_SCOPED_FULLSCREEN_VISIBILITY_H_

#include <windows.h>

#include <map>

#include "base/basictypes.h"
#include "ui/views/views_export.h"

namespace views {

class VIEWS_EXPORT ScopedFullscreenVisibility {
 public:
  explicit ScopedFullscreenVisibility(HWND hwnd);
  ~ScopedFullscreenVisibility();

  
  
  static bool IsHiddenForFullscreen(HWND hwnd);

 private:
  typedef std::map<HWND, int> FullscreenHWNDs;

  HWND hwnd_;

  static FullscreenHWNDs* full_screen_windows_;

  DISALLOW_COPY_AND_ASSIGN(ScopedFullscreenVisibility);
};

}  

#endif  

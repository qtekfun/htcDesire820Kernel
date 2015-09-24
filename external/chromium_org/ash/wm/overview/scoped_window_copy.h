// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_OVERVIEW_SCOPED_WINDOW_COPY_H_
#define ASH_WM_OVERVIEW_SCOPED_WINDOW_COPY_H_

#include "base/basictypes.h"

namespace aura {
class RootWindow;
class Window;
}

namespace ui {
class Layer;
}

namespace views {
class Widget;
}

namespace ash {

class CleanupWidgetAfterAnimationObserver;

class ScopedWindowCopy {
 public:
  ScopedWindowCopy(aura::Window* target_root, aura::Window* src_window);
  ~ScopedWindowCopy();

  aura::Window* GetWindow();

 private:
  
  views::Widget* widget_;

  
  
  ui::Layer* layer_;

  
  
  
  CleanupWidgetAfterAnimationObserver* cleanup_observer_;

  DISALLOW_COPY_AND_ASSIGN(ScopedWindowCopy);
};

}  

#endif  

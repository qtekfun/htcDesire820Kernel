// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_DRAG_WINDOW_CONTROLLER_H_
#define ASH_WM_DRAG_WINDOW_CONTROLLER_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "ui/gfx/display.h"
#include "ui/gfx/rect.h"

namespace aura {
class Window;
}

namespace ui {
class Layer;
}

namespace views {
class Widget;
}

namespace ash {
namespace internal {

class ASH_EXPORT DragWindowController {
 public:
  explicit DragWindowController(aura::Window* window);
  virtual ~DragWindowController();

  
  void SetDestinationDisplay(const gfx::Display& dst_display);

  
  
  
  
  void Show();

  
  void Hide();

  
  
  void SetBounds(const gfx::Rect& bounds);

  
  void SetOpacity(float opacity);

 private:
  FRIEND_TEST_ALL_PREFIXES(DragWindowResizerTest, DragWindowController);

  
  
  
  void CreateDragWidget(const gfx::Rect& bounds);

  
  
  void SetBoundsInternal(const gfx::Rect& bounds);

  
  void RecreateWindowLayers();

  
  aura::Window* window_;

  
  
  
  gfx::Display dst_display_;

  
  
  
  
  gfx::Rect bounds_;

  views::Widget* drag_widget_;

  
  
  ui::Layer* layer_;

  DISALLOW_COPY_AND_ASSIGN(DragWindowController);
};

}  
}  

#endif  

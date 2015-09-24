// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_RESIZE_SHADOW_H_
#define ASH_WM_RESIZE_SHADOW_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

namespace aura {
class Window;
}
namespace gfx {
class Rect;
}
namespace ui {
class Layer;
}
namespace views {
namespace corewm {
class ImageGrid;
}
}

namespace ash {
namespace internal {

class ResizeShadow {
 public:
  ResizeShadow();
  ~ResizeShadow();

  
  void Init(aura::Window* window);

  
  
  void ShowForHitTest(int hit_test);

  
  void Hide();

  
  void Layout(const gfx::Rect& bounds);

  int GetLastHitTestForTest() const {
    return last_hit_test_;
  }

 private:
  
  scoped_ptr<views::corewm::ImageGrid> image_grid_;

  
  
  int last_hit_test_;

  DISALLOW_COPY_AND_ASSIGN(ResizeShadow);
};

}  
}  

#endif  

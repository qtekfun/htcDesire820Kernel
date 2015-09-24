// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_DISPLAY_SHARED_DISPLAY_EDGE_INDICATOR_H_
#define ASH_DISPLAY_SHARED_DISPLAY_EDGE_INDICATOR_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/display.h"

namespace gfx {
class Rect;
class ThrobAnimation;
}

namespace views {
class View;
class Widget;
}

namespace ash {
namespace internal {

class ASH_EXPORT SharedDisplayEdgeIndicator : public gfx::AnimationDelegate {
 public:
  SharedDisplayEdgeIndicator();
  virtual ~SharedDisplayEdgeIndicator();

  
  
  
  void Show(const gfx::Rect& src_bounds, const gfx::Rect& dst_bounds);
  void Hide();

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;

 private:
  
  
  
  views::View* src_indicator_;
  views::View* dst_indicator_;

  
  scoped_ptr<gfx::ThrobAnimation> animation_;

  DISALLOW_COPY_AND_ASSIGN(SharedDisplayEdgeIndicator);
};

}  
}  

#endif  

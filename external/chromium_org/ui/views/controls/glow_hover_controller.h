// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_GLOW_HOVER_CONTROLLER_H_
#define UI_VIEWS_CONTROLS_GLOW_HOVER_CONTROLLER_H_

#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/animation/slide_animation.h"
#include "ui/views/views_export.h"

namespace gfx {
class Canvas;
class ImageSkia;
class Point;
}

namespace views {

class View;

class VIEWS_EXPORT GlowHoverController : public gfx::AnimationDelegate {
 public:
  enum Style {
    SUBTLE,
    PRONOUNCED
  };

  explicit GlowHoverController(views::View* view);
  virtual ~GlowHoverController();

  
  void SetAnimationContainer(gfx::AnimationContainer* container);

  
  
  void SetLocation(const gfx::Point& location);

  
  void Show(Style style);

  
  void Hide();

  
  void HideImmediately();

  
  double GetAnimationValue() const;

  
  
  bool ShouldDraw() const;

  
  
  void Draw(gfx::Canvas* canvas, const gfx::ImageSkia& mask_image) const;

  
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;

 private:
  
  views::View* view_;

  
  gfx::SlideAnimation animation_;

  
  gfx::Point location_;
  double opacity_scale_;

  DISALLOW_COPY_AND_ASSIGN(GlowHoverController);
};

}  

#endif  

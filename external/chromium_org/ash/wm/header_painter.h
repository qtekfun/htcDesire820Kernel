// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_HEADER_PAINTER_H_
#define ASH_WM_HEADER_PAINTER_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"  
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "ui/aura/window_observer.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/rect.h"

namespace aura {
class Window;
}
namespace gfx {
class Canvas;
class Font;
class ImageSkia;
class Point;
class Size;
class SlideAnimation;
}
namespace views {
class View;
class Widget;
}

namespace ash {
class FrameCaptionButtonContainerView;

class ASH_EXPORT HeaderPainter : public aura::WindowObserver,
                                 public gfx::AnimationDelegate {
 public:
  
  static int kActiveWindowOpacity;
  static int kInactiveWindowOpacity;
  static int kSoloWindowOpacity;

  enum HeaderMode {
    ACTIVE,
    INACTIVE
  };

  HeaderPainter();
  virtual ~HeaderPainter();

  
  void Init(views::Widget* frame,
            views::View* header_view,
            views::View* window_icon,
            FrameCaptionButtonContainerView* caption_button_container);

  
  
  
  static gfx::Rect GetBoundsForClientView(int header_height,
                                          const gfx::Rect& window_bounds);

  
  
  
  static gfx::Rect GetWindowBoundsForClientBounds(
      int header_height,
      const gfx::Rect& client_bounds);

  
  
  
  
  
  int NonClientHitTest(const gfx::Point& point) const;

  
  int GetMinimumHeaderWidth() const;

  
  int GetRightInset() const;

  
  int GetThemeBackgroundXInset() const;

  
  
  void PaintHeader(gfx::Canvas* canvas,
                   HeaderMode header_mode,
                   int theme_frame_id,
                   int theme_frame_overlay_id);

  
  
  
  void PaintHeaderContentSeparator(gfx::Canvas* canvas);

  
  int HeaderContentSeparatorSize() const;

  
  void PaintTitleBar(gfx::Canvas* canvas, const gfx::Font& title_font);

  
  
  
  void LayoutHeader(bool shorter_layout);

  
  
  void set_header_height(int header_height) {
    header_height_ = header_height;
  }

  
  int header_height() const {
    return header_height_;
  }

  
  void SchedulePaintForTitle(const gfx::Font& title_font);

  
  void OnThemeChanged();

  
  virtual void OnWindowDestroying(aura::Window* window) OVERRIDE;
  virtual void OnWindowBoundsChanged(aura::Window* window,
                                     const gfx::Rect& old_bounds,
                                     const gfx::Rect& new_bounds) OVERRIDE;

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(HeaderPainterTest, GetHeaderOpacity);
  FRIEND_TEST_ALL_PREFIXES(HeaderPainterTest, TitleIconAlignment);

  
  
  
  gfx::Rect GetHeaderLocalBounds() const;

  
  int GetTitleOffsetX() const;

  
  
  int GetCaptionButtonContainerCenterY() const;

  
  
  int GetHeaderOpacity(HeaderMode header_mode,
                       int theme_frame_id,
                       int theme_frame_overlay_id) const;

  
  int GetHeaderCornerRadius() const;

  
  
  void SchedulePaintForHeader();

  
  
  gfx::Rect GetTitleBounds(const gfx::Font& title_font);

  
  views::Widget* frame_;
  views::View* header_view_;
  views::View* window_icon_;  
  FrameCaptionButtonContainerView* caption_button_container_;
  aura::Window* window_;

  
  int header_height_;

  
  const gfx::ImageSkia* top_left_corner_;
  const gfx::ImageSkia* top_edge_;
  const gfx::ImageSkia* top_right_corner_;
  const gfx::ImageSkia* header_left_edge_;
  const gfx::ImageSkia* header_right_edge_;

  
  int previous_theme_frame_id_;
  int previous_theme_frame_overlay_id_;
  int previous_opacity_;

  
  int crossfade_theme_frame_id_;
  int crossfade_theme_frame_overlay_id_;
  int crossfade_opacity_;

  scoped_ptr<gfx::SlideAnimation> crossfade_animation_;

  DISALLOW_COPY_AND_ASSIGN(HeaderPainter);
};

}  

#endif  

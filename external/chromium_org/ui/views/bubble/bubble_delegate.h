// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_BUBBLE_BUBBLE_DELEGATE_H_
#define UI_VIEWS_BUBBLE_BUBBLE_DELEGATE_H_

#include "base/gtest_prod_util.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/views/bubble/bubble_border.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_delegate.h"
#include "ui/views/widget/widget_observer.h"

namespace gfx {
class Rect;
class SlideAnimation;
}

namespace views {

class BubbleFrameView;

class VIEWS_EXPORT BubbleDelegateView : public WidgetDelegateView,
                                        public gfx::AnimationDelegate,
                                        public WidgetObserver {
 public:
  BubbleDelegateView();
  BubbleDelegateView(View* anchor_view, BubbleBorder::Arrow arrow);
  virtual ~BubbleDelegateView();

  
  static Widget* CreateBubble(BubbleDelegateView* bubble_delegate);

  
  virtual BubbleDelegateView* AsBubbleDelegate() OVERRIDE;
  virtual bool CanActivate() const OVERRIDE;
  virtual bool ShouldShowCloseButton() const OVERRIDE;
  virtual View* GetContentsView() OVERRIDE;
  virtual NonClientFrameView* CreateNonClientFrameView(Widget* widget) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  virtual void OnWidgetDestroying(Widget* widget) OVERRIDE;
  virtual void OnWidgetVisibilityChanging(Widget* widget, bool visible)
      OVERRIDE;
  virtual void OnWidgetVisibilityChanged(Widget* widget, bool visible)
      OVERRIDE;
  virtual void OnWidgetActivationChanged(Widget* widget, bool active) OVERRIDE;
  virtual void OnWidgetBoundsChanged(Widget* widget,
                                     const gfx::Rect& new_bounds) OVERRIDE;

  bool close_on_esc() const { return close_on_esc_; }
  void set_close_on_esc(bool close_on_esc) { close_on_esc_ = close_on_esc; }

  bool close_on_deactivate() const { return close_on_deactivate_; }
  void set_close_on_deactivate(bool close) { close_on_deactivate_ = close; }

  View* GetAnchorView() const;
  Widget* anchor_widget() const { return anchor_widget_; }

  
  const gfx::Rect& anchor_rect() const { return anchor_rect_; }

  BubbleBorder::Arrow arrow() const { return arrow_; }
  void set_arrow(BubbleBorder::Arrow arrow) { arrow_ = arrow; }

  BubbleBorder::Shadow shadow() const { return shadow_; }
  void set_shadow(BubbleBorder::Shadow shadow) { shadow_ = shadow; }

  SkColor color() const { return color_; }
  void set_color(SkColor color) {
    color_ = color;
    color_explicitly_set_ = true;
  }

  const gfx::Insets& margins() const { return margins_; }
  void set_margins(const gfx::Insets& margins) { margins_ = margins; }

  const gfx::Insets& anchor_view_insets() const { return anchor_view_insets_; }
  void set_anchor_view_insets(const gfx::Insets& i) { anchor_view_insets_ = i; }

  gfx::NativeView parent_window() const { return parent_window_; }
  void set_parent_window(gfx::NativeView window) { parent_window_ = window; }

  bool use_focusless() const { return use_focusless_; }
  void set_use_focusless(bool focusless) { use_focusless_ = focusless; }

  bool accept_events() const { return accept_events_; }
  void set_accept_events(bool accept_events) { accept_events_ = accept_events; }

  bool border_accepts_events() const { return border_accepts_events_; }
  void set_border_accepts_events(bool event) { border_accepts_events_ = event; }

  bool adjust_if_offscreen() const { return adjust_if_offscreen_; }
  void set_adjust_if_offscreen(bool adjust) { adjust_if_offscreen_ = adjust; }

  bool move_with_anchor() const { return move_with_anchor_; }
  void set_move_with_anchor(bool move) { move_with_anchor_ = move; }

  
  virtual gfx::Rect GetAnchorRect();

  
  virtual void OnBeforeBubbleWidgetInit(Widget::InitParams* params,
                                        Widget* widget) const;

  
  
  void StartFade(bool fade_in);

  
  void ResetFade();

  
  
  void SetAlignment(BubbleBorder::BubbleAlignment alignment);

  
  void SetArrowPaintType(BubbleBorder::ArrowPaintType paint_type);

  
  
  
  void OnAnchorBoundsChanged();

 protected:
  
  virtual gfx::Rect GetBubbleBounds();

  
  virtual int GetFadeDuration();

  
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;
  virtual void OnNativeThemeChanged(const ui::NativeTheme* theme) OVERRIDE;

  
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;

  
  virtual void Init();

  
  
  
  
  void SetAnchorView(View* anchor_view);
  void SetAnchorRect(const gfx::Rect& rect);

  
  void SizeToContents();

  BubbleFrameView* GetBubbleFrameView() const;

 private:
  friend class BubbleBorderDelegate;

  FRIEND_TEST_ALL_PREFIXES(BubbleDelegateTest, CreateDelegate);
  FRIEND_TEST_ALL_PREFIXES(BubbleDelegateTest, NonClientHitTest);

  
  void UpdateColorsFromTheme(const ui::NativeTheme* theme);

  
  void HandleVisibilityChanged(Widget* widget, bool visible);

  
  scoped_ptr<gfx::SlideAnimation> fade_animation_;

  
  bool close_on_esc_;
  bool close_on_deactivate_;

  
  
  
  const int anchor_view_storage_id_;
  Widget* anchor_widget_;

  
  gfx::Rect anchor_rect_;

  
  bool move_with_anchor_;

  
  BubbleBorder::Arrow arrow_;

  
  BubbleBorder::Shadow shadow_;

  
  SkColor color_;
  bool color_explicitly_set_;

  
  gfx::Insets margins_;

  
  gfx::Insets anchor_view_insets_;

  
  int original_opacity_;

  
  Widget* border_widget_;

  
  bool use_focusless_;

  
  bool accept_events_;
  bool border_accepts_events_;

  
  
  bool adjust_if_offscreen_;

  
  gfx::NativeView parent_window_;

  DISALLOW_COPY_AND_ASSIGN(BubbleDelegateView);
};

}  

#endif  

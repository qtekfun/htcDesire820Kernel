// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_DROPDOWN_BAR_HOST_H_
#define CHROME_BROWSER_UI_VIEWS_DROPDOWN_BAR_HOST_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/native_web_keyboard_event.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"
#include "ui/views/controls/textfield/textfield.h"
#include "ui/views/focus/focus_manager.h"

class BrowserView;
class DropdownBarHostDelegate;
class DropdownBarView;

namespace content {
class WebContents;
}

namespace gfx {
class SlideAnimation;
}  

namespace views {
class ExternalFocusTracker;
class View;
class Widget;
}  

class DropdownBarHost : public ui::AcceleratorTarget,
                        public views::FocusChangeListener,
                        public gfx::AnimationDelegate {
 public:
  explicit DropdownBarHost(BrowserView* browser_view);
  virtual ~DropdownBarHost();

  
  
  
  
  
  void Init(views::View* host_view,
            views::View* view,
            DropdownBarHostDelegate* delegate);

  
  bool IsAnimating() const;
  
  bool IsVisible() const;
  
  void SetFocusAndSelection();
  
  void StopAnimation();

  
  virtual void Show(bool animate);
  
  virtual void Hide(bool animate);

  
  virtual gfx::Rect GetDialogPosition(gfx::Rect avoid_overlapping_rect) = 0;

  
  
  
  virtual void SetDialogPosition(const gfx::Rect& new_pos, bool no_redraw) = 0;

  
  virtual void OnWillChangeFocus(views::View* focused_before,
                                 views::View* focused_now) OVERRIDE;
  virtual void OnDidChangeFocus(views::View* focused_before,
                                views::View* focused_now) OVERRIDE;

  
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) = 0;
  virtual bool CanHandleAccelerators() const = 0;

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;

  
  
  
  static bool disable_animations_during_testing_;

  
  BrowserView* browser_view() const { return browser_view_; }

  
  
  
  
  virtual void RegisterAccelerators();

  
  
  virtual void UnregisterAccelerators();

 protected:
  
  
  virtual void OnVisibilityChanged();

  
  views::View* view() const { return view_; }

  
  views::ExternalFocusTracker* focus_tracker() const {
    return focus_tracker_.get();
  }

  
  void ResetFocusTracker();

  
  views::FocusManager* focus_manager() const { return focus_manager_; }

  
  views::Widget* host() const { return host_.get(); }

  
  int animation_offset() const { return animation_offset_; }

  
  
  
  
  
  virtual void GetWidgetBounds(gfx::Rect* bounds);

  
  
  
  
  
  
  
  void UpdateWindowEdges(const gfx::Rect& new_pos);

  
  void SetWidgetPositionNative(const gfx::Rect& new_pos, bool no_redraw);

  
  content::NativeWebKeyboardEvent GetKeyboardEvent(
      const content::WebContents* contents,
      const ui::KeyEvent& key_event);

  
  gfx::SlideAnimation* animation() {
    return animation_.get();
  }

 private:
  
  
  
  void SetHostViewNative(views::View* host_view);

  
  BrowserView* browser_view_;

  
  views::View* view_;
  DropdownBarHostDelegate* delegate_;

  
  
  int animation_offset_;

  
  scoped_ptr<gfx::SlideAnimation> animation_;

  
  views::FocusManager* focus_manager_;

  
  bool esc_accel_target_registered_;

  
  
  
  scoped_ptr<views::ExternalFocusTracker> focus_tracker_;

  
  
  scoped_ptr<views::Widget> host_;

  
  
  bool is_visible_;

  DISALLOW_COPY_AND_ASSIGN(DropdownBarHost);
};

#endif  

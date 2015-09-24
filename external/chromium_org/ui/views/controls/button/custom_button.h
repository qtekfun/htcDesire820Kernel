// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_BUTTON_CUSTOM_BUTTON_H_
#define UI_VIEWS_CONTROLS_BUTTON_CUSTOM_BUTTON_H_

#include "base/memory/scoped_ptr.h"
#include "ui/events/event_constants.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/views/controls/button/button.h"

namespace gfx {
class ThrobAnimation;
}

namespace views {

class CustomButtonStateChangedDelegate;

class VIEWS_EXPORT CustomButton : public Button,
                                  public gfx::AnimationDelegate {
 public:
  
  static const char kViewClassName[];

  static const CustomButton* AsCustomButton(const views::View* view);
  static CustomButton* AsCustomButton(views::View* view);

  virtual ~CustomButton();

  
  ButtonState state() const { return state_; }
  void SetState(ButtonState state);

  
  void StartThrobbing(int cycles_til_stop);

  
  void StopThrobbing();

  
  void SetAnimationDuration(int duration);

  void set_triggerable_event_flags(int triggerable_event_flags) {
    triggerable_event_flags_ = triggerable_event_flags;
  }
  int triggerable_event_flags() const { return triggerable_event_flags_; }

  
  
  void set_request_focus_on_press(bool value) {
    request_focus_on_press_ = value;
  }
  bool request_focus_on_press() const { return request_focus_on_press_; }

  
  void set_animate_on_state_change(bool value) {
    animate_on_state_change_ = value;
  }

  void SetHotTracked(bool is_hot_tracked);
  bool IsHotTracked() const;

  
  virtual void OnEnabledChanged() OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;
  virtual void OnMouseEntered(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseMoved(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;
  virtual bool OnKeyReleased(const ui::KeyEvent& event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;
  virtual void ShowContextMenu(const gfx::Point& p,
                               ui::MenuSourceType source_type) OVERRIDE;
  virtual void OnDragDone() OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual void VisibilityChanged(View* starting_from, bool is_visible) OVERRIDE;

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;

  
  void set_state_changed_delegate(CustomButtonStateChangedDelegate* delegate) {
    state_changed_delegate_.reset(delegate);
  }

 protected:
  
  explicit CustomButton(ButtonListener* listener);

  
  
  
  
  virtual void StateChanged();

  
  
  virtual bool IsTriggerableEvent(const ui::Event& event);

  
  
  
  virtual bool ShouldEnterPushedState(const ui::Event& event);

  
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;
  virtual void OnBlur() OVERRIDE;

  
  ButtonState state_;

  
  scoped_ptr<gfx::ThrobAnimation> hover_animation_;

 private:
  
  bool animate_on_state_change_;

  
  bool is_throbbing_;

  
  int triggerable_event_flags_;

  
  bool request_focus_on_press_;

  scoped_ptr<CustomButtonStateChangedDelegate> state_changed_delegate_;

  DISALLOW_COPY_AND_ASSIGN(CustomButton);
};

class VIEWS_EXPORT CustomButtonStateChangedDelegate {
public:
  virtual ~CustomButtonStateChangedDelegate() {}
  virtual void StateChanged(Button::ButtonState state) = 0;

protected:
  CustomButtonStateChangedDelegate() {}

private:
  DISALLOW_COPY_AND_ASSIGN(CustomButtonStateChangedDelegate);
};

}  

#endif  

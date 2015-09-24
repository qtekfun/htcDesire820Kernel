// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_ROOT_VIEW_H_
#define UI_VIEWS_WIDGET_ROOT_VIEW_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "ui/events/event_dispatcher.h"
#include "ui/views/focus/focus_manager.h"
#include "ui/views/focus/focus_search.h"
#include "ui/views/view.h"

namespace views {

namespace test {
class RootViewTestHelper;
class WidgetTest;
}

class Widget;

namespace internal {

class VIEWS_EXPORT RootView : public View,
                              public FocusTraversable,
                              public ui::EventDispatcherDelegate {
 public:
  static const char kViewClassName[];

  
  explicit RootView(Widget* widget);
  virtual ~RootView();

  

  
  
  void SetContentsView(View* contents_view);
  View* GetContentsView();

  
  void NotifyNativeViewHierarchyChanged();

  

  
  
  
  void DispatchKeyEvent(ui::KeyEvent* event);
  void DispatchScrollEvent(ui::ScrollEvent* event);
  void DispatchTouchEvent(ui::TouchEvent* event);
  virtual void DispatchGestureEvent(ui::GestureEvent* event);

  

  
  
  virtual void SetFocusTraversableParent(FocusTraversable* focus_traversable);

  
  virtual void SetFocusTraversableParentView(View* view);

  

  
  
  void ThemeChanged();

  
  
  void LocaleChanged();

  
  virtual FocusSearch* GetFocusSearch() OVERRIDE;
  virtual FocusTraversable* GetFocusTraversableParent() OVERRIDE;
  virtual View* GetFocusTraversableParentView() OVERRIDE;

  
  virtual const Widget* GetWidget() const OVERRIDE;
  virtual Widget* GetWidget() OVERRIDE;
  virtual bool IsDrawn() const OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;
  virtual void SchedulePaintInRect(const gfx::Rect& rect) OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;
  virtual void OnMouseMoved(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseWheel(const ui::MouseWheelEvent& event) OVERRIDE;
  virtual void SetMouseHandler(View* new_mouse_handler) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual void UpdateParentLayer() OVERRIDE;

 protected:
  
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;
  virtual void VisibilityChanged(View* starting_from, bool is_visible) OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual gfx::Vector2d CalculateOffsetToAncestorWithLayer(
      ui::Layer** layer_parent) OVERRIDE;
  virtual View::DragInfo* GetDragInfo() OVERRIDE;

 private:
  friend class ::views::View;
  friend class ::views::Widget;
  friend class ::views::test::RootViewTestHelper;
  friend class ::views::test::WidgetTest;

  

  
  
  
  
  
  void UpdateCursor(const ui::MouseEvent& event);

  
  
  
  void SetMouseLocationAndFlags(const ui::MouseEvent& event);

  void DispatchEventToTarget(View* target, ui::Event* event);

  
  
  
  void NotifyEnterExitOfDescendant(const ui::MouseEvent& event,
                                   ui::EventType type,
                                   View* view,
                                   View* sibling);

  
  
  void DispatchKeyEventStartAt(View* view, ui::KeyEvent* event);

  
  virtual bool CanDispatchToTarget(ui::EventTarget* target) OVERRIDE;

  

  

  
  Widget* widget_;

  

  
  View* mouse_pressed_handler_;

  
  View* mouse_move_handler_;

  
  
  View* last_click_handler_;

  
  bool explicit_mouse_handler_;

  
  
  int last_mouse_event_flags_;
  int last_mouse_event_x_;
  int last_mouse_event_y_;

  
  View* touch_pressed_handler_;

  
  
  
  View* gesture_handler_;

  
  View* scroll_gesture_handler_;

  

  
  FocusSearch focus_search_;

  
  

  
  FocusTraversable* focus_traversable_parent_;

  
  
  View* focus_traversable_parent_view_;

  View* event_dispatch_target_;

  

  
  View::DragInfo drag_info_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(RootView);
};

}  
}  

#endif  

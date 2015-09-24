// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_MENU_SUBMENU_VIEW_H_
#define UI_VIEWS_CONTROLS_MENU_SUBMENU_VIEW_H_

#include <string>

#include "base/compiler_specific.h"
#include "ui/views/animation/scroll_animator.h"
#include "ui/views/controls/menu/menu_delegate.h"
#include "ui/views/view.h"

namespace views {

class MenuHost;
class MenuItemView;
class MenuScrollViewContainer;

class VIEWS_EXPORT SubmenuView : public View,
                                 public ScrollDelegate {
 public:
  
  static const char kViewClassName[];

  
  explicit SubmenuView(MenuItemView* parent);
  virtual ~SubmenuView();

  
  
  int GetMenuItemCount();

  
  MenuItemView* GetMenuItemAt(int index);

  
  
  virtual void Layout() OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;

  
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  virtual void PaintChildren(gfx::Canvas* canvas) OVERRIDE;

  
  virtual bool GetDropFormats(
      int* formats,
      std::set<OSExchangeData::CustomFormat>* custom_formats) OVERRIDE;
  virtual bool AreDropTypesRequired() OVERRIDE;
  virtual bool CanDrop(const OSExchangeData& data) OVERRIDE;
  virtual void OnDragEntered(const ui::DropTargetEvent& event) OVERRIDE;
  virtual int OnDragUpdated(const ui::DropTargetEvent& event) OVERRIDE;
  virtual void OnDragExited() OVERRIDE;
  virtual int OnPerformDrop(const ui::DropTargetEvent& event) OVERRIDE;

  
  virtual bool OnMouseWheel(const ui::MouseWheelEvent& e) OVERRIDE;

  
  
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  
  bool IsShowing();

  
  
  void ShowAt(Widget* parent, const gfx::Rect& bounds, bool do_capture);

  
  void Reposition(const gfx::Rect& bounds);

  
  void Close();

  
  
  
  
  
  
  
  
  
  void Hide();

  
  
  void ReleaseCapture();

  
  virtual bool SkipDefaultKeyEventProcessing(const ui::KeyEvent& e) OVERRIDE;

  
  MenuItemView* GetMenuItem() const;

  
  void SetDropMenuItem(MenuItemView* item,
                       MenuDelegate::DropPosition position);

  
  
  
  bool GetShowSelection(MenuItemView* item);

  
  MenuScrollViewContainer* GetScrollViewContainer();

  
  
  
  void MenuHostDestroyed();

  
  
  int max_minor_text_width() const { return max_minor_text_width_; }

  
  
  void set_minimum_preferred_width(int minimum_preferred_width) {
    minimum_preferred_width_ = minimum_preferred_width;
  }

  
  bool resize_open_menu() const { return resize_open_menu_; }
  void set_resize_open_menu(bool resize_open_menu) {
    resize_open_menu_ = resize_open_menu;
  }

 protected:
  
  virtual const char* GetClassName() const OVERRIDE;

  
  
  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds) OVERRIDE;

  virtual void ChildPreferredSizeChanged(View* child) OVERRIDE;

 private:
  
  
  void PaintDropIndicator(gfx::Canvas* canvas,
                          MenuItemView* item,
                          MenuDelegate::DropPosition position);

  void SchedulePaintForDropIndicator(MenuItemView* item,
                                     MenuDelegate::DropPosition position);

  
  gfx::Rect CalculateDropIndicatorBounds(MenuItemView* item,
                                         MenuDelegate::DropPosition position);

  
  virtual bool OnScroll(float dx, float dy) OVERRIDE;

  
  MenuItemView* parent_menu_item_;

  
  
  MenuHost* host_;

  
  
  MenuItemView* drop_item_;

  
  MenuDelegate::DropPosition drop_position_;

  
  MenuScrollViewContainer* scroll_view_container_;

  
  int max_minor_text_width_;

  
  int minimum_preferred_width_;

  
  bool resize_open_menu_;

  
  scoped_ptr<ScrollAnimator> scroll_animator_;

  DISALLOW_COPY_AND_ASSIGN(SubmenuView);
};

}  

#endif  

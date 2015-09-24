// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_FOCUS_FOCUS_MANAGER_H_
#define UI_VIEWS_FOCUS_FOCUS_MANAGER_H_

#include <list>
#include <map>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/base/accelerators/accelerator_manager.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/views/views_export.h"


namespace ui {
class AcceleratorTarget;
class AcceleratorManager;
class EventHandler;
class KeyEvent;
}

namespace views {

class FocusManagerDelegate;
class FocusSearch;
class RootView;
class View;
class Widget;

class VIEWS_EXPORT FocusTraversable {
 public:
  
  
  virtual FocusSearch* GetFocusSearch() = 0;

  
  
  virtual FocusTraversable* GetFocusTraversableParent() = 0;

  
  
  
  virtual View* GetFocusTraversableParentView() = 0;

 protected:
  virtual ~FocusTraversable() {}
};

class VIEWS_EXPORT FocusChangeListener {
 public:
  
  virtual void OnWillChangeFocus(View* focused_before, View* focused_now) = 0;

  
  virtual void OnDidChangeFocus(View* focused_before, View* focused_now) = 0;

 protected:
  virtual ~FocusChangeListener() {}
};

class VIEWS_EXPORT FocusManager {
 public:
  
  enum FocusChangeReason {
    
    
    kReasonFocusTraversal,

    
    kReasonFocusRestore,

    
    
    kReasonDirectFocusChange
  };

  
  enum Direction {
    kForward,
    kBackward
  };

  enum FocusCycleWrappingBehavior {
    kWrap,
    kNoWrap
  };

  FocusManager(Widget* widget, FocusManagerDelegate* delegate);
  virtual ~FocusManager();

  
  
  
  bool OnKeyEvent(const ui::KeyEvent& event);

  
  
  bool ContainsView(View* view);

  
  void AdvanceFocus(bool reverse);

  
  View* GetFocusedView() { return focused_view_; }
  const View* GetFocusedView() const { return focused_view_; }

  
  
  
  void SetFocusedViewWithReason(View* view, FocusChangeReason reason);
  void SetFocusedView(View* view) {
    SetFocusedViewWithReason(view, kReasonDirectFocusChange);
  }

  
  FocusChangeReason focus_change_reason() const {
    return focus_change_reason_;
  }

  
  
  void ClearFocus();

  
  
  void ValidateFocusedView();

  
  
  
  void StoreFocusedView(bool clear_native_focus);

  
  
  
  bool RestoreFocusedView();

  
  
  
  void SetStoredFocusView(View* view);

  
  
  View* GetStoredFocusView();

  
  void ClearStoredFocusedView();

  
  bool is_changing_focus() const { return is_changing_focus_; }

  
  static void set_shortcut_handling_suspended(bool suspended) {
    shortcut_handling_suspended_ = suspended;
  }
  
  bool shortcut_handling_suspended() { return shortcut_handling_suspended_; }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void RegisterAccelerator(const ui::Accelerator& accelerator,
                           ui::AcceleratorManager::HandlerPriority priority,
                           ui::AcceleratorTarget* target);

  
  void UnregisterAccelerator(const ui::Accelerator& accelerator,
                             ui::AcceleratorTarget* target);

  
  void UnregisterAccelerators(ui::AcceleratorTarget* target);

  
  
  
  
  
  
  bool ProcessAccelerator(const ui::Accelerator& accelerator);

  
  
  void MaybeResetMenuKeyState(const ui::KeyEvent& key);

  
  
  
  
  void ViewRemoved(View* removed);

  
  
  void AddFocusChangeListener(FocusChangeListener* listener);
  void RemoveFocusChangeListener(FocusChangeListener* listener);

  
  
  
  ui::AcceleratorTarget* GetCurrentTargetForAccelerator(
      const ui::Accelerator& accelertor) const;

  
  bool HasPriorityHandler(const ui::Accelerator& accelerator) const;

  
  virtual void ClearNativeFocus();

  
  
  
  
  
  
  
  bool RotatePaneFocus(Direction direction, FocusCycleWrappingBehavior wrap);

  
  
  
  static bool IsTabTraversalKeyEvent(const ui::KeyEvent& key_event);

  
  
  
  static void set_arrow_key_traversal_enabled(bool enabled) {
    arrow_key_traversal_enabled_ = enabled;
  }
  
  static bool arrow_key_traversal_enabled() {
    return arrow_key_traversal_enabled_;
  }

  
  
  
  
  
  
  View* GetNextFocusableView(View* starting_view,
                             Widget* starting_widget,
                             bool reverse,
                             bool dont_loop);

 private:
  
  
  
  
  View* FindFocusableView(FocusTraversable* focus_traversable,
                          View* starting_view,
                          bool reverse);

  
  
  bool ProcessArrowKeyTraversal(const ui::KeyEvent& event);

  
  static bool shortcut_handling_suspended_;

  
  static bool arrow_key_traversal_enabled_;

  
  Widget* widget_;

  
  
  scoped_ptr<FocusManagerDelegate> delegate_;

  
  View* focused_view_;

  
  scoped_ptr<ui::AcceleratorManager> accelerator_manager_;

  
  
  int stored_focused_view_storage_id_;

  
  FocusChangeReason focus_change_reason_;

  
  ObserverList<FocusChangeListener, true> focus_change_listeners_;

  
  bool is_changing_focus_;

  DISALLOW_COPY_AND_ASSIGN(FocusManager);
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELF_SHELF_WIDGET_H_
#define ASH_SHELF_SHELF_WIDGET_H_

#include "ash/ash_export.h"
#include "ash/shelf/background_animator.h"
#include "ash/shelf/shelf_layout_manager_observer.h"
#include "ash/shelf/shelf_types.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_observer.h"

namespace aura {
class Window;
}

namespace ash {
class Launcher;

namespace internal {
class FocusCycler;
class StatusAreaWidget;
class ShelfLayoutManager;
class WorkspaceController;
}

class ASH_EXPORT ShelfWidget : public views::Widget,
                               public views::WidgetObserver,
                               public ShelfLayoutManagerObserver {
 public:
  ShelfWidget(
      aura::Window* shelf_container,
      aura::Window* status_container,
      internal::WorkspaceController* workspace_controller);
  virtual ~ShelfWidget();

  
  
  
  static bool ShelfAlignmentAllowed();

  void SetAlignment(ShelfAlignment alignmnet);
  ShelfAlignment GetAlignment() const;

  
  void SetPaintsBackground(ShelfBackgroundType background_type,
                           BackgroundAnimatorChangeType change_type);
  ShelfBackgroundType GetBackgroundType() const;

  
  void SetDimsShelf(bool dimming);
  bool GetDimsShelf() const;

  internal::ShelfLayoutManager* shelf_layout_manager() {
    return shelf_layout_manager_;
  }
  Launcher* launcher() const { return launcher_.get(); }
  internal::StatusAreaWidget* status_area_widget() const {
    return status_area_widget_;
  }

  void CreateLauncher();

  
  void SetLauncherVisibility(bool visible);
  bool IsLauncherVisible() const;

  
  void SetFocusCycler(internal::FocusCycler* focus_cycler);
  internal::FocusCycler* GetFocusCycler();

  
  
  void WillActivateAsFallback() { activating_as_fallback_ = true; }

  aura::Window* window_container() { return window_container_; }

  
  void ShutdownStatusAreaWidget();

  
  void ForceUndimming(bool force);

  
  virtual void OnWidgetActivationChanged(
      views::Widget* widget, bool active) OVERRIDE;

  
  
  int GetDimmingAlphaForTest();

  
  
  gfx::Rect GetDimmerBoundsForTest();

  
  void DisableDimmingAnimationsForTest();

  
  virtual void WillDeleteShelf() OVERRIDE;

 private:
  class DelegateView;

  internal::ShelfLayoutManager* shelf_layout_manager_;
  scoped_ptr<Launcher> launcher_;
  internal::StatusAreaWidget* status_area_widget_;

  
  
  DelegateView* delegate_view_;
  internal::BackgroundAnimator background_animator_;
  bool activating_as_fallback_;
  aura::Window* window_container_;
};

}  

#endif  

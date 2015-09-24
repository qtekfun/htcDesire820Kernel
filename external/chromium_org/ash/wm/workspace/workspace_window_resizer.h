// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_WORKSPACE_WINDOW_RESIZER_H_
#define ASH_WM_WORKSPACE_WINDOW_RESIZER_H_

#include <vector>

#include "ash/wm/window_resizer.h"
#include "ash/wm/workspace/magnetism_matcher.h"
#include "ash/wm/workspace/snap_types.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "ui/aura/window_tracker.h"

namespace ash {
namespace wm {
class WindowState;
}

namespace internal {

class DockedWindowLayoutManager;
class PhantomWindowController;
class SnapSizer;
class WindowSize;

class ASH_EXPORT WorkspaceWindowResizer : public WindowResizer {
 public:
  
  
  
  static const int kMinOnscreenSize;

  
  
  static const int kMinOnscreenHeight;

  
  
  static const int kScreenEdgeInset;

  
  
  static const int kStickyDistancePixels;

  virtual ~WorkspaceWindowResizer();

  static WorkspaceWindowResizer* Create(
      aura::Window* window,
      const gfx::Point& location_in_parent,
      int window_component,
      aura::client::WindowMoveSource source,
      const std::vector<aura::Window*>& attached_windows);

  
  virtual void Drag(const gfx::Point& location_in_parent,
                    int event_flags) OVERRIDE;
  virtual void CompleteDrag(int event_flags) OVERRIDE;
  virtual void RevertDrag() OVERRIDE;
  virtual aura::Window* GetTarget() OVERRIDE;
  virtual const gfx::Point& GetInitialLocation() const OVERRIDE;

 private:
  WorkspaceWindowResizer(const Details& details,
                         const std::vector<aura::Window*>& attached_windows);

 private:
  friend class WorkspaceWindowResizerTest;

  
  
  gfx::Rect GetFinalBounds(const gfx::Rect& bounds) const;

  
  void LayoutAttachedWindows(gfx::Rect* bounds);

  
  
  
  
  
  
  
  
  
  
  
  int CalculateAttachedSizes(
      int delta,
      int available_size,
      std::vector<int>* sizes) const;

  
  
  
  int GrowFairly(int amount, std::vector<WindowSize>& sizes) const;

  
  
  void CalculateGrowthRatios(const std::vector<WindowSize*>& sizes,
                             std::vector<float>* out_ratios) const;

  
  void CreateBucketsForAttached(std::vector<WindowSize>* sizes) const;

  
  
  void MagneticallySnapToOtherWindows(gfx::Rect* bounds);

  
  
  void MagneticallySnapResizeToOtherWindows(gfx::Rect* bounds);

  
  
  
  
  bool UpdateMagnetismWindow(const gfx::Rect& bounds, uint32 edges);

  
  
  
  
  void AdjustBoundsForMainWindow(int snap_size, gfx::Rect* bounds);

  
  bool StickToWorkAreaOnMove(const gfx::Rect& work_area,
                             int sticky_size,
                             gfx::Rect* bounds) const;

  
  void StickToWorkAreaOnResize(const gfx::Rect& work_area,
                               int sticky_size,
                               gfx::Rect* bounds) const;

  
  
  int PrimaryAxisSize(const gfx::Size& size) const;
  int PrimaryAxisCoordinate(int x, int y) const;

  
  void UpdateSnapPhantomWindow(const gfx::Point& location,
                               const gfx::Rect& bounds);

  
  
  void RestackWindows();

  
  
  SnapType GetSnapType(const gfx::Point& location) const;

  
  
  void SetDraggedWindowDocked(bool should_dock);

  aura::Window* window() const { return details_.window; }

  wm::WindowState* window_state() { return details_.window_state; }

  const Details details_;

  const std::vector<aura::Window*> attached_windows_;

  bool did_lock_cursor_;

  
  bool did_move_or_resize_;

  
  bool initial_bounds_changed_by_user_;

  
  
  std::vector<int> initial_size_;

  
  int total_min_;

  
  int total_initial_size_;

  
  
  scoped_ptr<PhantomWindowController> snap_phantom_window_controller_;

  
  scoped_ptr<SnapSizer> snap_sizer_;

  
  SnapType snap_type_;

  
  
  
  int num_mouse_moves_since_bounds_change_;

  
  gfx::Point last_mouse_location_;

  
  aura::Window* magnetism_window_;

  
  aura::WindowTracker window_tracker_;

  
  
  MatchedEdge magnetism_edge_;

  
  DockedWindowLayoutManager* dock_layout_;

  
  
  base::WeakPtrFactory<WorkspaceWindowResizer> weak_ptr_factory_;

  
  static WorkspaceWindowResizer* instance_;

  DISALLOW_COPY_AND_ASSIGN(WorkspaceWindowResizer);
};

}  
}  

#endif  

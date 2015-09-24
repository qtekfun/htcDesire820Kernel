// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_PANELS_PANEL_DRAG_CONTROLLER_H_
#define CHROME_BROWSER_UI_PANELS_PANEL_DRAG_CONTROLLER_H_

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/panels/panel_collection.h"
#include "ui/gfx/vector2d.h"

class Panel;
class PanelCollection;
class PanelManager;
namespace gfx {
class Point;
class Rect;
}

class PanelDragController {
 public:
  explicit PanelDragController(PanelManager* panel_manager);
  ~PanelDragController();

  
  
  void StartDragging(Panel* panel, const gfx::Point& mouse_location);
  void Drag(const gfx::Point& mouse_location);
  void EndDragging(bool cancelled);

  
  void OnPanelClosed(Panel* panel);

  bool is_dragging() const { return dragging_panel_ != NULL; }
  Panel* dragging_panel() const { return dragging_panel_; }

  
  static int GetDetachDockedPanelThresholdForTesting();
  static int GetDockDetachedPanelThresholdForTesting();
  static int GetGluePanelDistanceThresholdForTesting();
  static int GetGluePanelOverlapThresholdForTesting();
  static int GetSnapPanelToScreenEdgeThresholdForTesting();

 private:
  enum GlueAction {
    STACK,
    SNAP
  };

  enum GlueEdge {
    TOP_EDGE,
    BOTTOM_EDGE,
    LEFT_EDGE,
    RIGHT_EDGE
  };

  gfx::Point GetPanelPositionForMouseLocation(
      const gfx::Point& mouse_location) const;

  
  
  void TryDetach(const gfx::Point& target_position);
  void TryDock(const gfx::Point& target_position);
  void TryStack(const gfx::Point& target_position);
  bool TryUnstackFromTop(const gfx::Point& target_position);
  bool TryUnstackFromBottom(const gfx::Point& target_position);
  void TrySnap(gfx::Point* target_position);

  
  
  
  
  Panel* FindPanelToGlue(const gfx::Point& potential_position,
                         GlueAction action,
                         gfx::Rect* target_bounds,
                         GlueEdge* target_edge) const;

  
  
  void MovePanelAndBelowToCollection(
      Panel* panel,
      PanelCollection* target_collection,
      PanelCollection::PositioningMask positioning_mask) const;

  PanelManager* panel_manager_;  
  bool panel_stacking_enabled_;

  
  Panel* dragging_panel_;

  
  PanelCollection* dragging_panel_original_collection_;

  
  
  gfx::Vector2d offset_from_mouse_location_on_drag_start_;

  DISALLOW_COPY_AND_ASSIGN(PanelDragController);
};

#endif  

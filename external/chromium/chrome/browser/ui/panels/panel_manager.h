// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_PANELS_PANEL_MANAGER_H_
#define CHROME_BROWSER_UI_PANELS_PANEL_MANAGER_H_
#pragma once

#include <deque>
#include <vector>
#include "base/basictypes.h"
#include "base/scoped_ptr.h"
#include "ui/gfx/rect.h"

class Browser;
class Panel;

class PanelManager {
 public:
  
  static PanelManager* GetInstance();

  ~PanelManager();

  
  void OnDisplayChanged();

  
  
  Panel* CreatePanel(Browser* browser);

  
  
  
  void Remove(Panel* panel);

  
  
  void MinimizeAll();

  
  
  void RestoreAll();

  
  void RemoveAllActive();

  
  bool AreAllMinimized() const;

  
  void StartDragging(Panel* panel);
  void Drag(int delta_x);
  void EndDragging(bool cancelled);

  
  int active_count() const { return active_panels_.size(); }

 private:
  typedef std::vector<Panel*> ActivePanels;
  typedef std::deque<Panel*> PendingPanels;

  PanelManager();

  
  void DelayedRemove();

  
  void DoRemove(Panel* panel);

  
  
  
  void Rearrange(ActivePanels::iterator iter_to_start);

  
  void ProcessPending();

  
  
  
  
  bool ComputeBoundsForNextPanel(gfx::Rect* bounds, bool allow_size_change);

  
  void DragNegative(int delta_x);
  void DragPositive(int delta_x);

  
  ActivePanels active_panels_;

  
  PendingPanels pending_panels_;

  
  
  std::vector<Panel*> panels_pending_to_remove_;

  
  int max_width_;
  int max_height_;
  int min_x_;
  int current_x_;
  int bottom_edge_y_;

  
  size_t dragging_panel_index_;

  
  
  int dragging_panel_original_x_;

  
  
  
  gfx::Rect dragging_panel_bounds_;

  DISALLOW_COPY_AND_ASSIGN(PanelManager);
};

#endif  

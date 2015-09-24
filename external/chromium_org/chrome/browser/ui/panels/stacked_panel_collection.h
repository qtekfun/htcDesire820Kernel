// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_PANELS_STACKED_PANEL_COLLECTION_H_
#define CHROME_BROWSER_UI_PANELS_STACKED_PANEL_COLLECTION_H_

#include <list>
#include <vector>
#include "base/basictypes.h"
#include "chrome/browser/ui/panels/native_panel_stack_window.h"
#include "chrome/browser/ui/panels/panel_collection.h"
#include "chrome/browser/ui/panels/panel_constants.h"
#include "ui/gfx/rect.h"

class PanelManager;
namespace gfx {
class Vector2d;
}

class StackedPanelCollection : public PanelCollection,
                               public NativePanelStackWindowDelegate {
 public:
  typedef std::list<Panel*> Panels;

  explicit StackedPanelCollection(PanelManager* panel_manager);
  virtual ~StackedPanelCollection();

  
  virtual void OnDisplayChanged() OVERRIDE;
  virtual void RefreshLayout() OVERRIDE;
  virtual void AddPanel(Panel* panel,
                        PositioningMask positioning_mask) OVERRIDE;
  virtual void RemovePanel(Panel* panel, RemovalReason reason) OVERRIDE;
  virtual void CloseAll() OVERRIDE;
  virtual void ResizePanelWindow(
      Panel* panel,
      const gfx::Size& preferred_window_size) OVERRIDE;
  virtual panel::Resizability GetPanelResizability(
      const Panel* panel) const OVERRIDE;
  virtual void OnPanelResizedByMouse(Panel* panel,
                                     const gfx::Rect& new_bounds) OVERRIDE;
  virtual void OnPanelAttentionStateChanged(Panel* panel) OVERRIDE;
  virtual void OnPanelTitlebarClicked(Panel* panel,
                                      panel::ClickModifier modifier) OVERRIDE;
  virtual void ActivatePanel(Panel* panel) OVERRIDE;
  virtual void MinimizePanel(Panel* panel) OVERRIDE;
  virtual void RestorePanel(Panel* panel) OVERRIDE;
  virtual void OnMinimizeButtonClicked(Panel* panel,
                                       panel::ClickModifier modifier) OVERRIDE;
  virtual void OnRestoreButtonClicked(Panel* panel,
                                      panel::ClickModifier modifier) OVERRIDE;
  virtual bool CanShowMinimizeButton(const Panel* panel) const OVERRIDE;
  virtual bool CanShowRestoreButton(const Panel* panel) const OVERRIDE;
  virtual bool IsPanelMinimized(const Panel* panel) const OVERRIDE;
  virtual bool UsesAlwaysOnTopPanels() const OVERRIDE;
  virtual void SavePanelPlacement(Panel* panel) OVERRIDE;
  virtual void RestorePanelToSavedPlacement() OVERRIDE;
  virtual void DiscardSavedPanelPlacement()  OVERRIDE;
  virtual void UpdatePanelOnCollectionChange(Panel* panel) OVERRIDE;
  virtual void OnPanelExpansionStateChanged(Panel* panel) OVERRIDE;
  virtual void OnPanelActiveStateChanged(Panel* panel) OVERRIDE;
  virtual gfx::Rect GetInitialPanelBounds(
      const gfx::Rect& requested_bounds) const OVERRIDE;

  Panel* GetPanelAbove(Panel* panel) const;
  Panel* GetPanelBelow(Panel* panel) const;
  bool HasPanel(Panel* panel) const;

  void MoveAllDraggingPanelsInstantly(const gfx::Vector2d& delta_origin);

  bool IsMinimized() const;
  bool IsAnimatingPanelBounds(Panel* panel) const;

  
  
  
  
  int GetMaximiumAvailableBottomSpace() const;

  int num_panels() const { return panels_.size(); }
  const Panels& panels() const { return panels_; }
  Panel* top_panel() const { return panels_.empty() ? NULL : panels_.front(); }
  Panel* bottom_panel() const {
    return panels_.empty() ? NULL : panels_.back();
  }
  Panel* most_recently_active_panel() const {
    return most_recently_active_panels_.empty() ?
        NULL : most_recently_active_panels_.front();
  }

 private:
  struct PanelPlacement {
    Panel* panel;
    gfx::Point position;
    
    
    
    
    
    Panel* top_panel;

    PanelPlacement() : panel(NULL), top_panel(NULL) { }
  };

  
  virtual base::string16 GetTitle() const OVERRIDE;
  virtual gfx::Image GetIcon() const OVERRIDE;
  virtual void PanelBoundsBatchUpdateCompleted() OVERRIDE;

  
  gfx::Rect GetEnclosingBounds() const;

  
  
  
  gfx::Rect GetWorkArea() const;

  
  
  void RefreshLayoutWithTopPanelStartingAt(const gfx::Point& start_position,
                                           int common_width);

  
  
  
  int MinimizePanelsForSpace(int needed_space);

  
  
  
  int GetCurrentAvailableTopSpace() const;

  
  
  
  int GetCurrentAvailableBottomSpace() const;

  
  void MinimizeAll();
  void RestoreAll(Panel* panel_clicked);

  void UpdatePanelCornerStyle(Panel* panel);

  NativePanelStackWindow* GetStackWindowForPanel(Panel* panel) const;

  PanelManager* panel_manager_;

  
  
  

  
  
  
  
  
  
  NativePanelStackWindow* primary_stack_window_;

  
  
  
  
  
  NativePanelStackWindow* secondary_stack_window_;

  Panels panels_;  

  
  
  Panels most_recently_active_panels_;

  
  PanelPlacement saved_panel_placement_;

  bool minimizing_all_;  

  DISALLOW_COPY_AND_ASSIGN(StackedPanelCollection);
};

#endif  

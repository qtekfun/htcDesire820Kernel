// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_PANELS_DOCKED_PANEL_COLLECTION_H_
#define CHROME_BROWSER_UI_PANELS_DOCKED_PANEL_COLLECTION_H_

#include <list>
#include <set>
#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/ui/panels/display_settings_provider.h"
#include "chrome/browser/ui/panels/panel.h"
#include "chrome/browser/ui/panels/panel_collection.h"
#include "chrome/browser/ui/panels/panel_mouse_watcher_observer.h"
#include "ui/gfx/rect.h"

class PanelManager;

class DockedPanelCollection :
    public PanelCollection,
    public PanelMouseWatcherObserver,
    public DisplaySettingsProvider::DesktopBarObserver {
 public:
  typedef std::list<Panel*> Panels;

  explicit DockedPanelCollection(PanelManager* panel_manager);
  virtual ~DockedPanelCollection();

  
  virtual void OnDisplayChanged() OVERRIDE;

  
  
  
  
  virtual void RefreshLayout() OVERRIDE;

  
  
  virtual void AddPanel(Panel* panel,
                        PositioningMask positioning_mask) OVERRIDE;
  virtual void RemovePanel(Panel* pane, RemovalReason reasonl) OVERRIDE;
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
  virtual void DiscardSavedPanelPlacement() OVERRIDE;
  virtual void UpdatePanelOnCollectionChange(Panel* panel) OVERRIDE;
  virtual void OnPanelExpansionStateChanged(Panel* panel) OVERRIDE;
  virtual void OnPanelActiveStateChanged(Panel* panel) OVERRIDE;
  virtual gfx::Rect GetInitialPanelBounds(
      const gfx::Rect& requested_bounds) const OVERRIDE;

  
  
  bool ShouldBringUpTitlebars(int mouse_x, int mouse_y) const;

  
  void BringUpOrDownTitlebars(bool bring_up);

  
  
  
  int GetBottomPositionForExpansionState(
      Panel::ExpansionState expansion_state) const;

  
  
  int WidthToDisplayPanelInCollection(bool is_for_active_panel,
                                      double squeeze_factor,
                                      int full_width) const;

  bool HasPanel(Panel* panel) const;

  
  
  int num_panels() const { return panels_.size(); }
  const Panels& panels() const { return panels_; }
  Panel* last_panel() const { return panels_.empty() ? NULL : panels_.back(); }

  gfx::Rect work_area() const { return work_area_; }

  int StartingRightPosition() const;

#ifdef UNIT_TEST
  int minimized_panel_count() const {return minimized_panel_count_; }
#endif

 private:
  friend class DockedPanelDragHandler;

  enum TitlebarAction {
    NO_ACTION,
    BRING_UP,
    BRING_DOWN
  };

  struct PanelPlacement {
    Panel* panel;
    
    
    Panel* left_panel;

    PanelPlacement() : panel(NULL), left_panel(NULL) { }
  };

  
  virtual void OnMouseMove(const gfx::Point& mouse_position) OVERRIDE;

  
  virtual void OnAutoHidingDesktopBarVisibilityChanged(
      DisplaySettingsProvider::DesktopBarAlignment alignment,
      DisplaySettingsProvider::DesktopBarVisibility visibility) OVERRIDE;
  virtual void OnAutoHidingDesktopBarThicknessChanged(
      DisplaySettingsProvider::DesktopBarAlignment alignment,
      int thickness) OVERRIDE;

  
  void ScheduleLayoutRefresh();

  
  void UpdateMinimizedPanelCount();

  
  void MinimizeAll();
  void RestoreAll();

  
  
  
  void AdjustPanelBoundsPerExpansionState(Panel* panel,
      gfx::Rect* panel_bounds);

  
  void DoBringUpOrDownTitlebars(bool bring_up);
  
  
  void DelayedBringUpOrDownTitlebarsCheck();

  
  
  gfx::Point GetDefaultPositionForPanel(const gfx::Size& full_size) const;

  int GetRightMostAvailablePosition() const;

  PanelManager* panel_manager_;  

  
  gfx::Rect work_area_;

  Panels panels_;

  int minimized_panel_count_;
  bool are_titlebars_up_;

  bool minimizing_all_;  

  
  
  TitlebarAction delayed_titlebar_action_;

  
  PanelPlacement saved_panel_placement_;

  static const int kPanelsHorizontalSpacing = 4;

  
  base::WeakPtrFactory<DockedPanelCollection> titlebar_action_factory_;

  
  base::WeakPtrFactory<DockedPanelCollection> refresh_action_factory_;

  DISALLOW_COPY_AND_ASSIGN(DockedPanelCollection);
};

#endif  

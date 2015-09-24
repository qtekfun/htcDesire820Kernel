// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_PANELS_DETACHED_PANEL_COLLECTION_H_
#define CHROME_BROWSER_UI_PANELS_DETACHED_PANEL_COLLECTION_H_

#include <list>
#include "base/basictypes.h"
#include "chrome/browser/ui/panels/panel.h"
#include "chrome/browser/ui/panels/panel_collection.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"

class PanelManager;

class DetachedPanelCollection : public PanelCollection {
 public:
  typedef std::list<Panel*> Panels;
  typedef bool (*PanelsComparer)(Panel* panel1, Panel* panel2);

  explicit DetachedPanelCollection(PanelManager* panel_manager);
  virtual ~DetachedPanelCollection();

  
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
  virtual void DiscardSavedPanelPlacement() OVERRIDE;
  virtual void UpdatePanelOnCollectionChange(Panel* panel) OVERRIDE;
  virtual void OnPanelExpansionStateChanged(Panel* panel) OVERRIDE;
  virtual void OnPanelActiveStateChanged(Panel* panel) OVERRIDE;
  virtual gfx::Rect GetInitialPanelBounds(
      const gfx::Rect& requested_bounds) const OVERRIDE;

  bool HasPanel(Panel* panel) const;

  void SortPanels(PanelsComparer comparer);

  int num_panels() const { return panels_.size(); }
  const Panels& panels() const { return panels_; }

  
  
  gfx::Point GetDefaultPanelOrigin();

 private:
  
  
  void ComputeNextDefaultPanelOrigin();

  struct PanelPlacement {
    Panel* panel;
    gfx::Point position;

    PanelPlacement() : panel(NULL) { }
  };

  PanelManager* panel_manager_;  

  
  Panels panels_;

  
  PanelPlacement saved_panel_placement_;

  
  
  gfx::Point default_panel_origin_;

  DISALLOW_COPY_AND_ASSIGN(DetachedPanelCollection);
};

#endif  

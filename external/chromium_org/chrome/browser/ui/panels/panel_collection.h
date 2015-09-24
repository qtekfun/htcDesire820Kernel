// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_PANELS_PANEL_COLLECTION_H_
#define CHROME_BROWSER_UI_PANELS_PANEL_COLLECTION_H_

#include "chrome/browser/ui/panels/panel_constants.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"

class Panel;

class PanelCollection {
 public:
  
  enum Type {
    DETACHED,  
    DOCKED,    
    STACKED,   
  };

  
  enum PositioningMask {
    
    
    DEFAULT_POSITION = 0x0,
    
    KNOWN_POSITION = 0x1,
    
    
    TOP_POSITION = 0x2,
    
    DO_NOT_UPDATE_BOUNDS = 0x4,
    
    
    
    DELAY_LAYOUT_REFRESH = 0x8,
    
    NO_LAYOUT_REFRESH = 0x10,
    
    
    COLLAPSE_TO_FIT = 0x20
  };

  enum RemovalReason {
    PANEL_CLOSED,
    PANEL_CHANGED_COLLECTION
  };

  Type type() const { return type_; }

  
  virtual void OnDisplayChanged() = 0;

  
  
  virtual void RefreshLayout() = 0;

  
  
  virtual void AddPanel(Panel* panel, PositioningMask positioning_mask) = 0;

  
  
  
  virtual void RemovePanel(Panel* panel, RemovalReason reason) = 0;

  
  virtual void CloseAll() = 0;

  
  
  
  
  
  virtual void ResizePanelWindow(Panel* panel,
                                 const gfx::Size& preferred_window_size) = 0;

  
  
  virtual panel::Resizability GetPanelResizability(
      const Panel* panel) const = 0;

  
  
  
  
  virtual void OnPanelResizedByMouse(Panel* panel,
                                     const gfx::Rect& new_bounds) = 0;

  
  
  
  virtual void OnPanelAttentionStateChanged(Panel* panel) = 0;

  
  
  virtual void OnPanelTitlebarClicked(Panel* panel,
                                      panel::ClickModifier modifier) = 0;

  
  virtual void OnPanelExpansionStateChanged(Panel* panel) = 0;

  
  virtual void OnPanelActiveStateChanged(Panel* panel) = 0;

  
  virtual void ActivatePanel(Panel* panel) = 0;

  
  virtual void MinimizePanel(Panel* panel) = 0;
  virtual void RestorePanel(Panel* panel) = 0;

  
  
  virtual void OnMinimizeButtonClicked(Panel* panel,
                                       panel::ClickModifier modifier) = 0;
  virtual void OnRestoreButtonClicked(Panel* panel,
                                      panel::ClickModifier modifier) = 0;

  
  
  virtual bool CanShowMinimizeButton(const Panel* panel) const = 0;
  virtual bool CanShowRestoreButton(const Panel* panel) const = 0;

  virtual bool IsPanelMinimized(const Panel* panel) const = 0;

  virtual bool UsesAlwaysOnTopPanels() const = 0;

  
  
  
  
  
  virtual void SavePanelPlacement(Panel* panel) = 0;
  virtual void RestorePanelToSavedPlacement() = 0;
  virtual void DiscardSavedPanelPlacement() = 0;

  
  
  
  virtual void UpdatePanelOnCollectionChange(Panel* panel) = 0;

  
  virtual gfx::Rect GetInitialPanelBounds(
      const gfx::Rect& requested_bounds) const = 0;

 protected:
  explicit PanelCollection(Type type);
  virtual ~PanelCollection();

  const Type type_;  
};

#endif  

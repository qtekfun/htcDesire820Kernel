// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_PANELS_PANEL_MANAGER_H_
#define CHROME_BROWSER_UI_PANELS_PANEL_MANAGER_H_

#include <list>
#include <vector>
#include "base/basictypes.h"
#include "base/lazy_instance.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/panels/display_settings_provider.h"
#include "chrome/browser/ui/panels/panel.h"
#include "chrome/browser/ui/panels/panel_collection.h"
#include "chrome/browser/ui/panels/panel_constants.h"
#include "ui/gfx/rect.h"

class DetachedPanelCollection;
class DockedPanelCollection;
class GURL;
class PanelDragController;
class PanelResizeController;
class PanelMouseWatcher;
class StackedPanelCollection;

class PanelManager : public DisplaySettingsProvider::DisplayObserver,
                     public DisplaySettingsProvider::FullScreenObserver {
 public:
  typedef std::list<StackedPanelCollection*> Stacks;

  enum CreateMode {
    CREATE_AS_DOCKED,  
    CREATE_AS_DETACHED  
  };

  
  static PanelManager* GetInstance();

  
  
  static void SetDisplaySettingsProviderForTesting(
      DisplaySettingsProvider* provider);

  
  static bool ShouldUsePanels(const std::string& extension_id);

  
  static bool IsPanelStackingEnabled();

  
  
  
  
  static bool CanUseSystemMinimize();

  
  gfx::Point GetDefaultDetachedPanelOrigin();

  
  
  
  
  
  
  
  
  Panel* CreatePanel(const std::string& app_name,
                     Profile* profile,
                     const GURL& url,
                     const gfx::Rect& requested_bounds,
                     CreateMode mode);

  
  void CloseAll();

  
  void OnPanelClosed(Panel* panel);

  
  StackedPanelCollection* CreateStack();

  
  void RemoveStack(StackedPanelCollection* stack);

  
  
  int GetMaxPanelWidth(const gfx::Rect& work_area) const;
  int GetMaxPanelHeight(const gfx::Rect& work_area) const;

  
  
  void StartDragging(Panel* panel, const gfx::Point& mouse_location);
  void Drag(const gfx::Point& mouse_location);
  void EndDragging(bool cancelled);

  
  
  void StartResizingByMouse(Panel* panel, const gfx::Point& mouse_location,
                            panel::ResizingSides sides);
  void ResizeByMouse(const gfx::Point& mouse_location);
  void EndResizingByMouse(bool cancelled);

  
  void OnPanelExpansionStateChanged(Panel* panel);

  
  void MovePanelToCollection(Panel* panel,
                             PanelCollection* target_collection,
                             PanelCollection::PositioningMask positioning_mask);

  
  
  bool ShouldBringUpTitlebars(int mouse_x, int mouse_y) const;

  
  void BringUpOrDownTitlebars(bool bring_up);

  std::vector<Panel*> GetDetachedAndStackedPanels() const;

  int num_panels() const;
  std::vector<Panel*> panels() const;

  const Stacks& stacks() const { return stacks_; }
  int num_stacks() const { return stacks_.size(); }

  PanelDragController* drag_controller() const {
    return drag_controller_.get();
  }

#ifdef UNIT_TEST
  PanelResizeController* resize_controller() const {
    return resize_controller_.get();
  }
#endif

  DisplaySettingsProvider* display_settings_provider() const {
    return display_settings_provider_.get();
  }

  PanelMouseWatcher* mouse_watcher() const {
    return panel_mouse_watcher_.get();
  }

  DetachedPanelCollection* detached_collection() const {
    return detached_collection_.get();
  }

  DockedPanelCollection* docked_collection() const {
    return docked_collection_.get();
  }

  
  
  static inline double AdjustTimeInterval(double interval) {
    if (shorten_time_intervals_)
      return interval / 500.0;
    else
      return interval;
  }


  bool auto_sizing_enabled() const {
    return auto_sizing_enabled_;
  }

  
  void OnPanelAnimationEnded(Panel* panel);

#ifdef UNIT_TEST
  static void shorten_time_intervals_for_testing() {
    shorten_time_intervals_ = true;
  }

  void set_display_settings_provider(
      DisplaySettingsProvider* display_settings_provider) {
    display_settings_provider_.reset(display_settings_provider);
  }

  void enable_auto_sizing(bool enabled) {
    auto_sizing_enabled_ = enabled;
  }

  void SetMouseWatcherForTesting(PanelMouseWatcher* watcher) {
    SetMouseWatcher(watcher);
  }
#endif

 private:
  friend struct base::DefaultLazyInstanceTraits<PanelManager>;

  PanelManager();
  virtual ~PanelManager();

  void Initialize(DisplaySettingsProvider* provider);

  
  virtual void OnDisplayChanged() OVERRIDE;

  
  virtual void OnFullScreenModeChanged(bool is_full_screen) OVERRIDE;

  
  
  
  
  
  PanelCollection* GetCollectionForNewPanel(
      Panel* new_panel,
      const gfx::Rect& bounds,
      CreateMode mode,
      PanelCollection::PositioningMask* positioning_mask);

  
  void SetMouseWatcher(PanelMouseWatcher* watcher);

  
  static bool shorten_time_intervals_;

  scoped_ptr<DetachedPanelCollection> detached_collection_;
  scoped_ptr<DockedPanelCollection> docked_collection_;
  Stacks stacks_;

  scoped_ptr<PanelDragController> drag_controller_;
  scoped_ptr<PanelResizeController> resize_controller_;

  
  
  
  scoped_ptr<PanelMouseWatcher> panel_mouse_watcher_;

  scoped_ptr<DisplaySettingsProvider> display_settings_provider_;

  
  
  
  bool auto_sizing_enabled_;

  DISALLOW_COPY_AND_ASSIGN(PanelManager);
};

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WINDOW_SIZER_H_
#define CHROME_BROWSER_UI_WINDOW_SIZER_H_
#pragma once

#include <vector>

#include "base/basictypes.h"
#include "ui/gfx/rect.h"

class Browser;

class WindowSizer {
 public:
  class MonitorInfoProvider;
  class StateProvider;

  
  WindowSizer(StateProvider* state_provider,
              MonitorInfoProvider* monitor_info_provider);
  virtual ~WindowSizer();

  
  
  static MonitorInfoProvider* CreateDefaultMonitorInfoProvider();

  
  
  class MonitorInfoProvider {
   public:
    MonitorInfoProvider();
    virtual ~MonitorInfoProvider();

    
    virtual gfx::Rect GetPrimaryMonitorWorkArea() const = 0;

    
    virtual gfx::Rect GetPrimaryMonitorBounds() const = 0;

    
    
    virtual gfx::Rect GetMonitorWorkAreaMatching(
        const gfx::Rect& match_rect) const = 0;

    
    
    virtual gfx::Point GetBoundsOffsetMatching(
        const gfx::Rect& match_rect) const = 0;

    
    
    
    virtual void UpdateWorkAreas() = 0;

    
    size_t GetMonitorCount() const {
      return work_areas_.size();
    }

    
    
    gfx::Rect GetWorkAreaAt(size_t index) const {
      return work_areas_[index];
    }

   protected:
    std::vector<gfx::Rect> work_areas_;
  };

  
  
  class StateProvider {
   public:
    virtual ~StateProvider() { }

    
    
    virtual bool GetPersistentState(gfx::Rect* bounds,
                                    bool* maximized,
                                    gfx::Rect* work_area) const = 0;

    
    
    
    virtual bool GetLastActiveWindowState(gfx::Rect* bounds) const = 0;
  };

  
  
  
  
  
  
  
  
  
  
  
  void DetermineWindowBounds(const gfx::Rect& specified_bounds,
                             gfx::Rect* bounds,
                             bool* maximized) const;

  
  
  
  
  
  static void GetBrowserWindowBounds(const std::string& app_name,
                                     const gfx::Rect& specified_bounds,
                                     const Browser* browser,
                                     gfx::Rect* window_bounds,
                                     bool* maximized);

  
  static gfx::Point GetDefaultPopupOrigin(const gfx::Size& size);

  
  
  static const int kWindowTilePixels;

 private:
  
  enum Edge { TOP, LEFT, BOTTOM, RIGHT };

  explicit WindowSizer(const std::string& app_name);

  void Init(StateProvider* state_provider,
            MonitorInfoProvider* monitor_info_provider);

  
  
  
  bool GetLastWindowBounds(gfx::Rect* bounds) const;

  
  
  
  
  bool GetSavedWindowBounds(gfx::Rect* bounds, bool* maximized) const;

  
  
  
  void GetDefaultWindowBounds(gfx::Rect* default_bounds) const;

  
  
  bool PositionIsOffscreen(int position, Edge edge) const;

  
  
  
  
  
  
  
  
  void AdjustBoundsToBeVisibleOnMonitorContaining(
      const gfx::Rect& other_bounds,
      const gfx::Rect& saved_work_area,
      gfx::Rect* bounds) const;

  
  StateProvider* state_provider_;
  MonitorInfoProvider* monitor_info_provider_;

  DISALLOW_COPY_AND_ASSIGN(WindowSizer);
};

#endif  


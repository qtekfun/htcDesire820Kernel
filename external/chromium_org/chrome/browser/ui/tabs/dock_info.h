// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TABS_DOCK_INFO_H_
#define CHROME_BROWSER_UI_TABS_DOCK_INFO_H_

#include <set>

#include "chrome/browser/ui/host_desktop.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"

class DockInfo {
 public:
  
  enum Type {
    
    NONE,

    
    
    LEFT_OF_WINDOW,
    RIGHT_OF_WINDOW,
    BOTTOM_OF_WINDOW,
    TOP_OF_WINDOW,

    
    MAXIMIZE,

    
    LEFT_HALF,
    RIGHT_HALF,
    BOTTOM_HALF
  };

  DockInfo() : type_(NONE), window_(NULL), in_enable_area_(false) {}

  
  
  
  static bool IsCloseToPoint(const gfx::Point& screen_loc,
                             int x,
                             int y,
                             bool* in_enable_area);

  
  static bool IsCloseToMonitorPoint(const gfx::Point& screen_loc,
                                    int x,
                                    int y,
                                    DockInfo::Type type,
                                    bool* in_enable_area);

  
  static int popup_width();
  static int popup_height();

  
  
  
  
  
  
  static DockInfo GetDockInfoAtPoint(chrome::HostDesktopType host_desktop_type,
                                     const gfx::Point& screen_point,
                                     const std::set<gfx::NativeView>& ignore);

  
  
  
  
  static gfx::NativeWindow GetLocalProcessWindowAtPoint(
      chrome::HostDesktopType host_desktop_type,
      const gfx::Point& screen_point,
      const std::set<gfx::NativeView>& ignore);

  static int GetHotSpotDeltaY();

  
  
  bool IsValidForPoint(const gfx::Point& screen_point);

  
  
  
  
  bool GetNewWindowBounds(gfx::Rect* new_window_bounds,
                          bool* maximize_new_window) const;

  
  
  void AdjustOtherWindowBounds() const;

  
  void set_type(Type type) { type_ = type; }
  Type type() const { return type_; }

  
  
  void set_window(gfx::NativeWindow window) { window_ = window; }
  gfx::NativeWindow window() const { return window_; }

  
  void set_hot_spot(const gfx::Point& hot_spot) { hot_spot_ = hot_spot; }
  const gfx::Point& hot_spot() const { return hot_spot_; }

  
  void set_monitor_bounds(const gfx::Rect& monitor_bounds) {
    monitor_bounds_ = monitor_bounds;
  }
  const gfx::Rect& monitor_bounds() const { return monitor_bounds_; }

  
  gfx::Rect GetPopupRect() const;

  
  
  
  
  
  
  
  
  void set_in_enable_area(bool in_enable_area) {
    in_enable_area_ = in_enable_area;
  }
  bool in_enable_area() const { return in_enable_area_; }

  
  
  bool equals(const DockInfo& other) const {
    return type_ == other.type_ && window_ == other.window_ &&
           monitor_bounds_ == other.monitor_bounds_;
  }

  
  
  
  bool CheckMonitorPoint(const gfx::Point& screen_loc,
                         int x,
                         int y,
                         Type type);

 private:
  
  bool GetWindowBounds(gfx::Rect* bounds) const;

  
  void SizeOtherWindowTo(const gfx::Rect& bounds) const;

  Type type_;
  gfx::NativeWindow window_;
  gfx::Point hot_spot_;
  gfx::Rect monitor_bounds_;
  bool in_enable_area_;
};

#endif  

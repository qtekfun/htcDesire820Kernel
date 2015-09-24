// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WINDOW_SIZER_WINDOW_SIZER_H_
#define CHROME_BROWSER_UI_WINDOW_SIZER_WINDOW_SIZER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/host_desktop.h"
#include "ui/base/ui_base_types.h"
#include "ui/gfx/rect.h"

class Browser;

namespace gfx {
class Display;
class Screen;
}

class WindowSizer {
 public:
  class StateProvider;
  class TargetDisplayProvider;

  
  
  WindowSizer(scoped_ptr<StateProvider> state_provider,
              scoped_ptr<TargetDisplayProvider> target_display_provider,
              const Browser* browser);

  
  
  WindowSizer(scoped_ptr<StateProvider> state_provider,
              scoped_ptr<TargetDisplayProvider> target_display_provider,
              gfx::Screen* screen,
              const Browser* browser);

  virtual ~WindowSizer();

  
  
  class StateProvider {
   public:
    virtual ~StateProvider() {}

    
    
    
    
    virtual bool GetPersistentState(gfx::Rect* bounds,
                                    gfx::Rect* work_area,
                                    ui::WindowShowState* show_state) const = 0;

    
    
    
    
    
    virtual bool GetLastActiveWindowState(
        gfx::Rect* bounds,
        ui::WindowShowState* show_state) const = 0;
  };

  
  
  class TargetDisplayProvider {
    public:
      virtual ~TargetDisplayProvider() {}
      virtual gfx::Display GetTargetDisplay(const gfx::Screen* screen,
                                            const gfx::Rect& bounds) const = 0;
  };

  
  
  
  
  
  
  // |show_state| will be overwritten and return the initial visual state of
  
  void DetermineWindowBoundsAndShowState(
      const gfx::Rect& specified_bounds,
      gfx::Rect* bounds,
      ui::WindowShowState* show_state) const;

  
  
  
  
  
  static void GetBrowserWindowBoundsAndShowState(
      const std::string& app_name,
      const gfx::Rect& specified_bounds,
      const Browser* browser,
      gfx::Rect* window_bounds,
      ui::WindowShowState* show_state);

  
  static gfx::Point GetDefaultPopupOrigin(const gfx::Size& size,
                                          chrome::HostDesktopType type);

  
  
  static const int kWindowTilePixels;

 private:
  
  enum Edge { TOP, LEFT, BOTTOM, RIGHT };

  
  
  
  
  bool GetLastActiveWindowBounds(gfx::Rect* bounds,
                                 ui::WindowShowState* show_state) const;

  
  
  
  
  
  bool GetSavedWindowBounds(gfx::Rect* bounds,
                            ui::WindowShowState* show_state) const;

  
  
  
  
  void GetDefaultWindowBounds(const gfx::Display& display,
                              gfx::Rect* default_bounds) const;

  
  
  
  
  
  
  
  
  void AdjustBoundsToBeVisibleOnDisplay(
      const gfx::Display& display,
      const gfx::Rect& saved_work_area,
      gfx::Rect* bounds) const;

  
  
  
  gfx::Display GetTargetDisplay(const gfx::Rect& bounds) const;

#if defined(USE_ASH)
  
  
  
  
  void GetTabbedBrowserBoundsAsh(gfx::Rect* bounds_in_screen,
                                 ui::WindowShowState* show_state) const;
#endif

  
  
  ui::WindowShowState GetWindowDefaultShowState() const;

#if defined(USE_ASH)
  bool IsTabbedBrowserInAsh() const;
  bool IsPopupBrowserInAsh() const;
#endif

  
  scoped_ptr<StateProvider> state_provider_;
  scoped_ptr<TargetDisplayProvider> target_display_provider_;
  gfx::Screen* screen_;  

  
  const Browser* browser_;

  DISALLOW_COPY_AND_ASSIGN(WindowSizer);
};

#endif  

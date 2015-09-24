// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_PANELS_DISPLAY_SETTINGS_PROVIDER_H_
#define CHROME_BROWSER_UI_PANELS_DISPLAY_SETTINGS_PROVIDER_H_

#include "base/observer_list.h"
#include "base/timer/timer.h"
#include "ui/gfx/rect.h"

class DisplaySettingsProvider {
 public:
  
  
  enum DesktopBarAlignment {
    DESKTOP_BAR_ALIGNED_BOTTOM = 0,
    DESKTOP_BAR_ALIGNED_LEFT = 1,
    DESKTOP_BAR_ALIGNED_RIGHT = 2
  };

  
  enum DesktopBarVisibility {
    DESKTOP_BAR_VISIBLE,
    DESKTOP_BAR_ANIMATING,
    DESKTOP_BAR_HIDDEN
  };

  class DisplayObserver {
   public:
    virtual void OnDisplayChanged() = 0;
  };

  class DesktopBarObserver {
   public:
    virtual void OnAutoHidingDesktopBarVisibilityChanged(
        DesktopBarAlignment alignment, DesktopBarVisibility visibility) = 0;
    virtual void OnAutoHidingDesktopBarThicknessChanged(
        DesktopBarAlignment alignment, int thickness) = 0;
  };

  class FullScreenObserver {
   public:
    virtual void OnFullScreenModeChanged(bool is_full_screen) = 0;
  };

  static DisplaySettingsProvider* Create();

  virtual ~DisplaySettingsProvider();

  
  void AddDisplayObserver(DisplayObserver* observer);
  void RemoveDisplayObserver(DisplayObserver* observer);

  void AddDesktopBarObserver(DesktopBarObserver* observer);
  void RemoveDesktopBarObserver(DesktopBarObserver* observer);

  void AddFullScreenObserver(FullScreenObserver* observer);
  void RemoveFullScreenObserver(FullScreenObserver* observer);

  
  
  
  
  
  
  
  
  

  
  virtual gfx::Rect GetPrimaryDisplayArea() const;

  
  virtual gfx::Rect GetPrimaryWorkArea() const;

  
  
  virtual gfx::Rect GetDisplayAreaMatching(const gfx::Rect& bounds) const;

  
  
  virtual gfx::Rect GetWorkAreaMatching(const gfx::Rect& bounds) const;

  
  
  
  
  virtual void OnDisplaySettingsChanged();

  
  
  virtual bool IsAutoHidingDesktopBarEnabled(DesktopBarAlignment alignment);

  
  
  
  
  virtual int GetDesktopBarThickness(DesktopBarAlignment alignment) const;

  
  
  virtual DesktopBarVisibility GetDesktopBarVisibility(
      DesktopBarAlignment alignment) const;

  ObserverList<DisplayObserver>& display_observers() {
    return display_observers_;
  }

  ObserverList<DesktopBarObserver>& desktop_bar_observers() {
    return desktop_bar_observers_;
  }

  ObserverList<FullScreenObserver>& full_screen_observers() {
    return full_screen_observers_;
  }

  bool is_full_screen() const { return is_full_screen_; }

 protected:
  enum FullScreenCheckMode {
    ASSUME_FULLSCREEN_ON,
    ASSUME_FULLSCREEN_OFF,
    PERFORM_FULLSCREEN_CHECK
  };

  DisplaySettingsProvider();

  
  virtual bool NeedsPeriodicFullScreenCheck() const;

  
  virtual bool IsFullScreen();

  
  void CheckFullScreenMode(FullScreenCheckMode check_mode);

 private:
  
  ObserverList<DisplayObserver> display_observers_;
  ObserverList<DesktopBarObserver> desktop_bar_observers_;
  ObserverList<FullScreenObserver> full_screen_observers_;

  
  bool is_full_screen_;

  
  base::RepeatingTimer<DisplaySettingsProvider> full_screen_mode_timer_;

  DISALLOW_COPY_AND_ASSIGN(DisplaySettingsProvider);
};

#endif  

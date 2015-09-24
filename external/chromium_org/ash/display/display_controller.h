// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_DISPLAY_DISPLAY_CONTROLLER_H_
#define ASH_DISPLAY_DISPLAY_CONTROLLER_H_

#include <map>
#include <vector>

#include "ash/ash_export.h"
#include "ash/display/display_manager.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/time/time.h"
#include "ui/aura/root_window_observer.h"
#include "ui/aura/window.h"
#include "ui/gfx/display_observer.h"
#include "ui/gfx/point.h"

namespace aura {
class Display;
class RootWindow;
}

namespace base {
class Value;
template <typename T> class JSONValueConverter;
}

namespace gfx {
class Display;
class Insets;
}

namespace ash {
namespace internal {
class DisplayInfo;
class DisplayManager;
class FocusActivationStore;
class MirrorWindowController;
class RootWindowController;
class VirtualKeyboardWindowController;
}

class ASH_EXPORT DisplayController : public gfx::DisplayObserver,
                                     public aura::RootWindowObserver,
                                     public internal::DisplayManager::Delegate {
 public:
  class ASH_EXPORT Observer {
   public:
    
    
    virtual void OnDisplayConfigurationChanging() {}

    
    
    virtual void OnDisplayConfigurationChanged() {};

   protected:
    virtual ~Observer() {}
  };

  DisplayController();
  virtual ~DisplayController();

  void Start();
  void Shutdown();

  
  
  static const gfx::Display& GetPrimaryDisplay();

  
  
  static int GetNumDisplays();

  internal::MirrorWindowController* mirror_window_controller() {
    return mirror_window_controller_.get();
  }

  internal::VirtualKeyboardWindowController*
      virtual_keyboard_window_controller() {
    return virtual_keyboard_window_controller_.get();
  }

  
  void InitPrimaryDisplay();

  
  void InitSecondaryDisplays();

  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  aura::Window* GetPrimaryRootWindow();

  
  aura::Window* GetRootWindowForDisplayId(int64 id);

  
  void ToggleMirrorMode();

  
  void SwapPrimaryDisplay();

  
  
  void SetPrimaryDisplayId(int64 id);

  
  
  void SetPrimaryDisplay(const gfx::Display& display);

  
  void CloseChildWindows();

  
  
  aura::Window::Windows GetAllRootWindows();

  
  
  std::vector<internal::RootWindowController*> GetAllRootWindowControllers();

  
  
  gfx::Insets GetOverscanInsets(int64 display_id) const;
  void SetOverscanInsets(int64 display_id, const gfx::Insets& insets_in_dip);

  
  
  void EnsurePointerInDisplays();

  
  bool UpdateWorkAreaOfDisplayNearestWindow(const aura::Window* window,
                                            const gfx::Insets& insets);

  
  const gfx::Display& GetDisplayNearestPoint(
      const gfx::Point& point) const;

  
  const gfx::Display& GetDisplayNearestWindow(
      const aura::Window* window) const;

  
  const gfx::Display& GetDisplayMatching(
      const gfx::Rect& match_rect)const;

  
  virtual void OnDisplayBoundsChanged(
      const gfx::Display& display) OVERRIDE;
  virtual void OnDisplayAdded(const gfx::Display& display) OVERRIDE;
  virtual void OnDisplayRemoved(const gfx::Display& display) OVERRIDE;

  
  virtual void OnRootWindowHostResized(const aura::RootWindow* root) OVERRIDE;

  
  virtual void CreateOrUpdateNonDesktopDisplay(
      const internal::DisplayInfo& info) OVERRIDE;
  virtual void CloseNonDesktopDisplay() OVERRIDE;
  virtual void PreDisplayConfigurationChange(bool clear_focus) OVERRIDE;
  virtual void PostDisplayConfigurationChange() OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(DisplayControllerTest, BoundsUpdated);
  FRIEND_TEST_ALL_PREFIXES(DisplayControllerTest, SecondaryDisplayLayout);
  friend class internal::DisplayManager;
  friend class internal::MirrorWindowController;

  
  
  aura::RootWindow* AddRootWindowForDisplay(const gfx::Display& display);

  void OnFadeOutForSwapDisplayFinished();

  void UpdateHostWindowNames();

  class DisplayChangeLimiter {
   public:
    DisplayChangeLimiter();

    
    void SetThrottleTimeout(int64 throttle_ms);

    bool IsThrottled() const;

   private:
    
    base::Time throttle_timeout_;

    DISALLOW_COPY_AND_ASSIGN(DisplayChangeLimiter);
  };

  
  
  scoped_ptr<DisplayChangeLimiter> limiter_;

  
  std::map<int64, aura::Window*> root_windows_;

  ObserverList<Observer> observers_;

  
  
  aura::Window* primary_root_window_for_replace_;

  scoped_ptr<internal::FocusActivationStore> focus_activation_store_;

  scoped_ptr<internal::MirrorWindowController> mirror_window_controller_;
  scoped_ptr<internal::VirtualKeyboardWindowController>
      virtual_keyboard_window_controller_;

  
  
  
  gfx::Point cursor_location_in_native_coords_for_restore_;

  DISALLOW_COPY_AND_ASSIGN(DisplayController);
};

}  

#endif  

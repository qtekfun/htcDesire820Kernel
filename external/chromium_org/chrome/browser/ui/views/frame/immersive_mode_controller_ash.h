// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_IMMERSIVE_MODE_CONTROLLER_ASH_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_IMMERSIVE_MODE_CONTROLLER_ASH_H_

#include "chrome/browser/ui/views/frame/immersive_mode_controller.h"

#include "ash/wm/immersive_fullscreen_controller.h"
#include "ash/wm/window_state_observer.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/gfx/rect.h"

namespace aura {
class Window;
}

class ImmersiveModeControllerAsh
    : public ImmersiveModeController,
      public ash::ImmersiveFullscreenController::Delegate,
      public ash::wm::WindowStateObserver,
      public content::NotificationObserver {
 public:
  ImmersiveModeControllerAsh();
  virtual ~ImmersiveModeControllerAsh();

  
  virtual void Init(BrowserView* browser_view) OVERRIDE;
  virtual void SetEnabled(bool enabled) OVERRIDE;
  virtual bool IsEnabled() const OVERRIDE;
  virtual bool ShouldHideTabIndicators() const OVERRIDE;
  virtual bool ShouldHideTopViews() const OVERRIDE;
  virtual bool IsRevealed() const OVERRIDE;
  virtual int GetTopContainerVerticalOffset(
      const gfx::Size& top_container_size) const OVERRIDE;
  virtual ImmersiveRevealedLock* GetRevealedLock(
      AnimateReveal animate_reveal) OVERRIDE WARN_UNUSED_RESULT;
  virtual void OnFindBarVisibleBoundsChanged(
      const gfx::Rect& new_visible_bounds_in_screen) OVERRIDE;
  virtual void SetupForTest() OVERRIDE;

 private:
  
  
  void EnableWindowObservers(bool enable);

  
  void LayoutBrowserRootView();

  
  
  
  
  
  void SetRenderWindowTopInsetsForTouch(int top_inset);

  
  
  bool UpdateTabIndicators();

  
  virtual void OnImmersiveRevealStarted() OVERRIDE;
  virtual void OnImmersiveRevealEnded() OVERRIDE;
  virtual void OnImmersiveFullscreenExited() OVERRIDE;
  virtual void SetVisibleFraction(double visible_fraction) OVERRIDE;
  virtual std::vector<gfx::Rect> GetVisibleBoundsInScreen() const OVERRIDE;

  
  virtual void OnWindowShowTypeChanged(
      ash::wm::WindowState* window_state,
      ash::wm::WindowShowType old_type) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  scoped_ptr<ash::ImmersiveFullscreenController> controller_;

  
  BrowserView* browser_view_;
  aura::Window* native_window_;

  
  
  bool observers_enabled_;

  
  
  
  bool use_tab_indicators_;

  
  
  gfx::Rect find_bar_visible_bounds_in_screen_;

  
  
  
  double visible_fraction_;

  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(ImmersiveModeControllerAsh);
};

#endif  

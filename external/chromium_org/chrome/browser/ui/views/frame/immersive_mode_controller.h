// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_IMMERSIVE_MODE_CONTROLLER_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_IMMERSIVE_MODE_CONTROLLER_H_

#include "base/compiler_specific.h"
#include "base/observer_list.h"

#if defined(USE_ASH)
#include "ash/wm/immersive_revealed_lock.h"
#endif

class BrowserView;

namespace gfx {
class Rect;
class Size;
}

#if defined(USE_ASH)
typedef ash::ImmersiveRevealedLock ImmersiveRevealedLock;
#else
class ImmersiveRevealedLock {
 public:
  ImmersiveRevealedLock() {}
  ~ImmersiveRevealedLock() {}
};
#endif

class ImmersiveModeController {
 public:
  enum AnimateReveal {
    ANIMATE_REVEAL_YES,
    ANIMATE_REVEAL_NO
  };

  class Observer {
   public:
    
    virtual void OnImmersiveRevealStarted() {}

    
    virtual void OnImmersiveModeControllerDestroyed() {}

   protected:
    virtual ~Observer() {}
  };

  ImmersiveModeController();
  virtual ~ImmersiveModeController();

  
  virtual void Init(BrowserView* browser_view) = 0;

  
  virtual void SetEnabled(bool enabled) = 0;
  virtual bool IsEnabled() const = 0;

  
  
  virtual bool ShouldHideTabIndicators() const = 0;

  
  virtual bool ShouldHideTopViews() const = 0;

  
  virtual bool IsRevealed() const = 0;

  
  
  
  
  
  
  virtual int GetTopContainerVerticalOffset(
      const gfx::Size& top_container_size) const = 0;

  
  
  
  
  
  
  
  
  
  
  virtual ImmersiveRevealedLock* GetRevealedLock(
      AnimateReveal animate_reveal) WARN_UNUSED_RESULT = 0;

  
  
  
  virtual void OnFindBarVisibleBoundsChanged(
      const gfx::Rect& new_visible_bounds_in_screen) = 0;

  
  
  
  virtual void SetupForTest() = 0;

  virtual void AddObserver(Observer* observer);
  virtual void RemoveObserver(Observer* observer);

 protected:
  ObserverList<Observer> observers_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ImmersiveModeController);
};

namespace chrome {

ImmersiveModeController* CreateImmersiveModeController();

}  

#endif  

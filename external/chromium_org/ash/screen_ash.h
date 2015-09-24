// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SCREEN_ASH_H_
#define ASH_SCREEN_ASH_H_

#include "ash/ash_export.h"
#include "base/compiler_specific.h"
#include "base/observer_list.h"
#include "ui/gfx/screen.h"

namespace gfx {
class Rect;
}

namespace ash {

class ASH_EXPORT ScreenAsh : public gfx::Screen {
 public:
  ScreenAsh();
  virtual ~ScreenAsh();

  
  
  
  static gfx::Display FindDisplayContainingPoint(const gfx::Point& point);

  
  
  static gfx::Rect GetMaximizedWindowBoundsInParent(aura::Window* window);

  
  static gfx::Rect GetDisplayBoundsInParent(aura::Window* window);

  
  static gfx::Rect GetDisplayWorkAreaBoundsInParent(aura::Window* window);

  
  
  static gfx::Rect ConvertRectToScreen(aura::Window* window,
                                       const gfx::Rect& rect);

  
  
  static gfx::Rect ConvertRectFromScreen(aura::Window* window,
                                         const gfx::Rect& rect);

  
  
  static const gfx::Display& GetSecondaryDisplay();

  
  
  static const gfx::Display& GetDisplayForId(int64 display_id);

  
  void NotifyBoundsChanged(const gfx::Display& display);
  void NotifyDisplayAdded(const gfx::Display& display);
  void NotifyDisplayRemoved(const gfx::Display& display);

 protected:
  
  virtual bool IsDIPEnabled() OVERRIDE;
  virtual gfx::Point GetCursorScreenPoint() OVERRIDE;
  virtual gfx::NativeWindow GetWindowUnderCursor() OVERRIDE;
  virtual gfx::NativeWindow GetWindowAtScreenPoint(const gfx::Point& point)
      OVERRIDE;
  virtual int GetNumDisplays() const OVERRIDE;
  virtual std::vector<gfx::Display> GetAllDisplays() const OVERRIDE;
  virtual gfx::Display GetDisplayNearestWindow(
      gfx::NativeView view) const OVERRIDE;
  virtual gfx::Display GetDisplayNearestPoint(
      const gfx::Point& point) const OVERRIDE;
  virtual gfx::Display GetDisplayMatching(
      const gfx::Rect& match_rect) const OVERRIDE;
  virtual gfx::Display GetPrimaryDisplay() const OVERRIDE;
  virtual void AddObserver(gfx::DisplayObserver* observer) OVERRIDE;
  virtual void RemoveObserver(gfx::DisplayObserver* observer) OVERRIDE;

 private:
  ObserverList<gfx::DisplayObserver> observers_;

  DISALLOW_COPY_AND_ASSIGN(ScreenAsh);
};

}  

#endif  

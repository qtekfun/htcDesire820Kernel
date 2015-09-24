// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_SCREEN_H_
#define UI_GFX_SCREEN_H_

#include <vector>

#include "base/basictypes.h"
#include "ui/gfx/display.h"
#include "ui/gfx/gfx_export.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"
#include "ui/gfx/screen_type_delegate.h"

namespace gfx {
class DisplayObserver;
class Rect;

class GFX_EXPORT Screen {
 public:
  
  
  static Screen* GetScreenFor(NativeView view);

  
  
  static Screen* GetNativeScreen();

  
  
  static void SetScreenInstance(ScreenType type, Screen* instance);

  
  
  static Screen* GetScreenByType(ScreenType type);

  
  
  static void SetScreenTypeDelegate(ScreenTypeDelegate* delegate);

  Screen();
  virtual ~Screen();

  
  virtual bool IsDIPEnabled() = 0;

  
  virtual gfx::Point GetCursorScreenPoint() = 0;

  
  virtual gfx::NativeWindow GetWindowUnderCursor() = 0;

  
  virtual gfx::NativeWindow GetWindowAtScreenPoint(const gfx::Point& point) = 0;

  
  
  
  virtual int GetNumDisplays() const = 0;

  
  virtual std::vector<gfx::Display> GetAllDisplays() const = 0;

  
  virtual gfx::Display GetDisplayNearestWindow(NativeView view) const = 0;

  
  virtual gfx::Display GetDisplayNearestPoint(
      const gfx::Point& point) const = 0;

  
  virtual gfx::Display GetDisplayMatching(
      const gfx::Rect& match_rect) const = 0;

  
  virtual gfx::Display GetPrimaryDisplay() const = 0;

  
  virtual void AddObserver(DisplayObserver* observer) = 0;
  virtual void RemoveObserver(DisplayObserver* observer) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(Screen);
};

Screen* CreateNativeScreen();

}  

#endif  

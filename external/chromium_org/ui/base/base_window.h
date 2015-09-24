// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_BASE_WINDOW_H_
#define UI_BASE_BASE_WINDOW_H_

#include "base/compiler_specific.h"
#include "ui/base/ui_base_types.h"  
#include "ui/gfx/native_widget_types.h"

namespace gfx {
class Rect;
}

class SkRegion;

namespace ui {

class UI_EXPORT BaseWindow {
 public:
  
  virtual bool IsActive() const = 0;

  
  virtual bool IsMaximized() const = 0;

  
  virtual bool IsMinimized() const = 0;

  
  virtual bool IsFullscreen() const = 0;

  
  
  static bool IsRestored(const BaseWindow& window);

  
  virtual gfx::NativeWindow GetNativeWindow() = 0;

  
  
  virtual gfx::Rect GetRestoredBounds() const = 0;

  
  virtual ui::WindowShowState GetRestoredState() const = 0;

  
  
  
  virtual gfx::Rect GetBounds() const = 0;

  
  virtual void Show() = 0;

  
  virtual void Hide() = 0;

  
  
  virtual void ShowInactive() = 0;

  
  
  virtual void Close() = 0;

  
  
  virtual void Activate() = 0;

  
  
  virtual void Deactivate() = 0;

  
  virtual void Maximize() = 0;
  virtual void Minimize() = 0;
  virtual void Restore() = 0;

  
  virtual void SetBounds(const gfx::Rect& bounds) = 0;

  
  
  virtual void FlashFrame(bool flash) = 0;

  
  virtual bool IsAlwaysOnTop() const = 0;

  
  
  virtual void SetAlwaysOnTop(bool always_on_top) = 0;
};

}  

#endif  

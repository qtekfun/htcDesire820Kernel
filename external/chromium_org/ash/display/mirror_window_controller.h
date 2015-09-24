// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_DISPLAY_MIRROR_WINDOW_CONTROLLER_H_
#define ASH_DISPLAY_MIRROR_WINDOW_CONTROLLER_H_

#include "ash/ash_export.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "ui/aura/root_window_observer.h"
#include "ui/gfx/display.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"
#include "ui/gfx/size.h"

namespace aura {
class RootWindow;
class RootWindowTransformer;
class Window;
}

namespace ui {
class Reflector;
}

namespace ash {
namespace test{
class MirrorWindowTestApi;
}

namespace internal {
class DisplayInfo;
class CursorWindowDelegate;

class ASH_EXPORT MirrorWindowController : public aura::RootWindowObserver {
 public:
  MirrorWindowController();
  virtual ~MirrorWindowController();

  
  
  void UpdateWindow(const DisplayInfo& display_info);

  
  
  void UpdateWindow();

  
  void Close();

  
  
  void UpdateCursorLocation();
  void SetMirroredCursor(gfx::NativeCursor cursor);
  void SetMirroredCursorVisibility(bool visible);

  
  virtual void OnRootWindowHostResized(const aura::RootWindow* root) OVERRIDE;

 private:
  friend class test::MirrorWindowTestApi;

  
  
  scoped_ptr<aura::RootWindowTransformer> CreateRootWindowTransformer() const;

  int current_cursor_type_;
  gfx::Display::Rotation current_cursor_rotation_;
  aura::Window* cursor_window_;  
  scoped_ptr<aura::RootWindow> root_window_;
  scoped_ptr<CursorWindowDelegate> cursor_window_delegate_;
  gfx::Point hot_point_;
  gfx::Size mirror_window_host_size_;
  scoped_refptr<ui::Reflector> reflector_;

  DISALLOW_COPY_AND_ASSIGN(MirrorWindowController);
};

}  
}  

#endif  

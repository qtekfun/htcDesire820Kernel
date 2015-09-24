// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_CURSOR_LOADER_UPDATER_H_
#define UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_CURSOR_LOADER_UPDATER_H_

#include "base/memory/scoped_ptr.h"
#include "ui/views/views_export.h"

namespace aura {
class RootWindow;
}

namespace gfx {
class Display;
}

namespace ui {
class CursorLoader;
}

namespace views {

class VIEWS_EXPORT DesktopCursorLoaderUpdater {
 public:
  virtual ~DesktopCursorLoaderUpdater() {}

  
  
  static scoped_ptr<DesktopCursorLoaderUpdater> Create();

  
  virtual void OnCreate(aura::RootWindow* window,
                        ui::CursorLoader* loader) = 0;

  
  
  virtual void OnDisplayUpdated(const gfx::Display& display,
                                ui::CursorLoader* loader) = 0;
};

}  

#endif  

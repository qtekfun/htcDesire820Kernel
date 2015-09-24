// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_CURSOR_LOADER_UPDATER_AURALINUX_H_
#define UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_CURSOR_LOADER_UPDATER_AURALINUX_H_

#include "base/compiler_specific.h"
#include "ui/views/widget/desktop_aura/desktop_cursor_loader_updater.h"

namespace views {

class DesktopCursorLoaderUpdaterAuraLinux : public DesktopCursorLoaderUpdater {
 public:
  DesktopCursorLoaderUpdaterAuraLinux();
  virtual ~DesktopCursorLoaderUpdaterAuraLinux();

  
  virtual void OnCreate(aura::RootWindow* window,
                        ui::CursorLoader* loader) OVERRIDE;
  virtual void OnDisplayUpdated(const gfx::Display& display,
                                ui::CursorLoader* loader) OVERRIDE;
};

}  

#endif  

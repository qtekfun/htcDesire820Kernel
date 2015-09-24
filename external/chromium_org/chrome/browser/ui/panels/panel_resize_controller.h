// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_PANELS_PANEL_RESIZE_CONTROLLER_H_
#define CHROME_BROWSER_UI_PANELS_PANEL_RESIZE_CONTROLLER_H_

#include <set>
#include "base/basictypes.h"
#include "chrome/browser/ui/panels/panel_constants.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"

class Panel;
class PanelManager;
namespace gfx {
  class Rect;
}

class PanelResizeController {
 public:
  explicit PanelResizeController(PanelManager* panel_manager);

  
  
  void StartResizing(Panel* panel,
                     const gfx::Point& mouse_location,
                     panel::ResizingSides sides);
  void Resize(const gfx::Point& mouse_location);

  
  Panel* EndResizing(bool cancelled);

  
  void OnPanelClosed(Panel* panel);

  bool IsResizing() const { return resizing_panel_ != NULL; }

 private:
  PanelManager* panel_manager_;  

  
  Panel* resizing_panel_;

  
  panel::ResizingSides sides_resized_;

  
  
  gfx::Point mouse_location_at_start_;

  
  gfx::Rect bounds_at_start_;

  DISALLOW_COPY_AND_ASSIGN(PanelResizeController);
};

#endif  
